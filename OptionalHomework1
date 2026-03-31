#include <ntddk.h>

PVOID g_NonPaged = NULL;
PVOID g_Paged = NULL;

VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
    UNREFERENCED_PARAMETER(DriverObject);

    if (g_NonPaged)
    {
        ExFreePool(g_NonPaged);
        DbgPrint("NonPaged memory freed.\n");
    }

    if (g_Paged)
    {
        ExFreePool(g_Paged);
        DbgPrint("Paged memory freed.\n");
    }

    DbgPrint("Driver unloaded.\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    DriverObject->DriverUnload = DriverUnload;

    // 1. Alocare NonPagedPool
    g_NonPaged = ExAllocatePoolWithTag(NonPagedPool, 512, 'tag1');
    if (g_NonPaged)
    {
        RtlFillMemory(g_NonPaged, 512, 0xAA);
        DbgPrint("NonPaged memory allocated and written.\n");
    }

    // 2. Alocare PagedPool
    g_Paged = ExAllocatePoolWithTag(PagedPool, 512, 'tag2');
    if (g_Paged)
    {
        RtlFillMemory(g_Paged, 512, 0xAA);
        DbgPrint("Paged memory allocated.\n");
    }

    // 3. Acces PagedPool la IRQL DISPATCH_LEVEL
    KIRQL oldIrql;
    KeRaiseIrql(DISPATCH_LEVEL, &oldIrql);

    if (g_Paged)
    {
        volatile UCHAR val = *((UCHAR*)g_Paged); // acces invalid posibil
        UNREFERENCED_PARAMETER(val);
    }

    KeLowerIrql(oldIrql);

    return STATUS_SUCCESS;
}
