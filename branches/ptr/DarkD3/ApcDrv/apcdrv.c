#include "ntddk.h"

#define FILE_DEVICE_APCDRV			0x00008005
#define IOCTL_APCDRV_SET_ALERTABLE2 CTL_CODE(FILE_DEVICE_APCDRV, 0x800, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)


#define DEVICE_NAME			L"\\Device\\Apcdrv"
#define DOS_DEVICE_NAME		L"\\DosDevices\\APCDRV"

//Debugging macro
#ifdef DBG

#define DPRINT(_x_) \
                DbgPrint("ApcDrv.sys: ");\
                DbgPrint _x_;
#else
#define DPRINT(_x_)
#endif

//Function prototypes for APCs
void KeInitializeApc( PKAPC Apc,
					  PKTHREAD Thread,
					  CCHAR ApcStateIndex,
					  PKKERNEL_ROUTINE KernelRoutine,
					  PKRUNDOWN_ROUTINE RundownRoutine,
					  PKNORMAL_ROUTINE NormalRoutine,
					  KPROCESSOR_MODE ApcMode,
					  PVOID NormalContext
					);


void KeInsertQueueApc(PKAPC Apc,
					  PVOID SystemArgument1,
					  PVOID SystemArgument2,
					  UCHAR unknown
					 );


void KernelApcCallBack( PKAPC Apc, 
						PKNORMAL_ROUTINE NormalRoutine, 
						PVOID NormalContext, 
						PVOID SystemArgument1, 
						PVOID SystemArgument2)
{
  KEVENT event;
  LARGE_INTEGER Timeout;

  Timeout.QuadPart = 0;

  //Set thread alertable
  KeDelayExecutionThread(UserMode, TRUE, &Timeout);

  DPRINT(("Freeing APC Object\n"));

  //Free kernel memory used by KAPC
  ExFreePool(Apc);   

  return;
}


void UserApcCallBack(PVOID arg1, PVOID arg2, PVOID arg3)
{
	return;
}

//Function prototypes
NTSTATUS	DriverEntry(IN PDRIVER_OBJECT  DriverObject,IN PUNICODE_STRING registryPath);
NTSTATUS	ApcDrvDispatch(IN PDEVICE_OBJECT DeviceObject,IN PIRP Irp);
VOID		ApcDrvUnload(IN PDRIVER_OBJECT DriverObject);
NTSTATUS	ApcDrvDeliverAPC(PETHREAD Thread);


NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	PDEVICE_OBJECT deviceObject = NULL;
	NTSTATUS       status;
	WCHAR          deviceNameBuffer[] = DEVICE_NAME;
	UNICODE_STRING deviceNameUnicodeString;
	WCHAR          deviceLinkBuffer[] = DOS_DEVICE_NAME;
	UNICODE_STRING deviceLinkUnicodeString;

	DPRINT(("DriverEntry\n"));

	RtlInitUnicodeString(&deviceNameUnicodeString, deviceNameBuffer);

	status = IoCreateDevice(DriverObject,0,&deviceNameUnicodeString, FILE_DEVICE_APCDRV, 0, TRUE, &deviceObject);

	if (!NT_SUCCESS(status))
	{
		DPRINT (("IoCreateDevice failed:%x\n", status));
		return status;
	}

	DriverObject->MajorFunction[IRP_MJ_CREATE]         =
	DriverObject->MajorFunction[IRP_MJ_CLOSE]          =
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ApcDrvDispatch;
	DriverObject->DriverUnload = ApcDrvUnload;

	RtlInitUnicodeString(&deviceLinkUnicodeString,deviceLinkBuffer);

	status = IoCreateSymbolicLink(&deviceLinkUnicodeString, &deviceNameUnicodeString);

	if (!NT_SUCCESS(status))
	{
		DPRINT (("IoCreateSymbolicLink failed\n"));
		IoDeleteDevice (deviceObject);
	}

	return status;
}


NTSTATUS ApcDrvDispatch(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	PIO_STACK_LOCATION	irpStack;
	PVOID				ioBuffer;
	ULONG				inputBufferLength;
	ULONG				outputBufferLength;
	ULONG				ioControlCode;
	NTSTATUS			ntStatus;
	PHANDLE				ph = NULL;
	PETHREAD			uThread = NULL;

	Irp->IoStatus.Status      = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	irpStack = IoGetCurrentIrpStackLocation(Irp);

	ioBuffer           = Irp->AssociatedIrp.SystemBuffer;
	inputBufferLength  = irpStack->Parameters.DeviceIoControl.InputBufferLength;
	outputBufferLength = irpStack->Parameters.DeviceIoControl.OutputBufferLength;

	switch (irpStack->MajorFunction)
	{
		case IRP_MJ_CREATE:
			DPRINT (("IRP_MJ_CREATE\n"));
			break;

		case IRP_MJ_CLOSE:
			DPRINT (("IRP_MJ_CLOSE\n"));
			break;

		case IRP_MJ_DEVICE_CONTROL:
			{
				ioControlCode = irpStack->Parameters.DeviceIoControl.IoControlCode;

				switch (ioControlCode)
				{
					case IOCTL_APCDRV_SET_ALERTABLE2:
						{
							if (inputBufferLength >= sizeof(PVOID))
							{
								ph = (PHANDLE) ioBuffer;
								Irp->IoStatus.Status =
									ObReferenceObjectByHandle(*((PHANDLE)ph),SYNCHRONIZE | THREAD_SUSPEND_RESUME | THREAD_SET_CONTEXT, NULL, UserMode, &uThread, NULL);

								if (NT_ERROR(Irp->IoStatus.Status))
								{
									DPRINT (("ObReferenceObjectByHandle Failed (%ld)\n", Irp->IoStatus.Status));
								}
								else
								{
									DPRINT (("uThread = 0x%lx\n", uThread));

									Irp->IoStatus.Status = ApcDrvDeliverAPC(uThread);

									ObDereferenceObject((PVOID) uThread);
								}
							}
							else
							{
								Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
								DPRINT (("Invalid parameter passed!\n"));
							}
						}
					}
					break;

					default:
						DPRINT (("Unknown IRP_MJ_DEVICE_CONTROL\n"));
						Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
					break;
			}
			break;
	}

	ntStatus = Irp->IoStatus.Status;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return ntStatus;
}

VOID ApcDrvUnload(IN PDRIVER_OBJECT DriverObject)
{
	WCHAR			deviceLinkBuffer[]  = DOS_DEVICE_NAME;
	UNICODE_STRING  deviceLinkUnicodeString;

	RtlInitUnicodeString(&deviceLinkUnicodeString, deviceLinkBuffer);
	IoDeleteSymbolicLink(&deviceLinkUnicodeString);
	IoDeleteDevice(DriverObject->DeviceObject);

	DPRINT(("Driver has been unloaded\n"));

	return;
}

NTSTATUS ApcDrvDeliverAPC(PETHREAD uThread)
{
	NTSTATUS   ntStatus = STATUS_SUCCESS;
	PKAPC      kApc;

	//Allocate an KAPC structure from NonPagedPool
	kApc = ExAllocatePool(NonPagedPool, sizeof(KAPC));

	if (kApc == NULL)
	{
		DPRINT (("ExAllocatePool returned NULL\n"));
		return !ntStatus;
	}

	KeInitializeApc
		(
			kApc,
			(PKTHREAD) uThread,
			0,
			(PKKERNEL_ROUTINE)&KernelApcCallBack,
			0,
			(PKNORMAL_ROUTINE)NULL,
			KernelMode,
			NULL
		);

	KeInsertQueueApc(kApc, NULL, NULL, 0);

	return ntStatus;
}