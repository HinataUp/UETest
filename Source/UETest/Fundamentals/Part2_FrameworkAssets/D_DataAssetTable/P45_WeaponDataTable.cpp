#include "P45_WeaponDataTable.h"

bool UP45_WeaponDataTableHelper::LookupWeapon(UDataTable* Table, FName RowName, FP45_WeaponData& OutData)
{
	if (!Table) { return false; }
	const FP45_WeaponData* Row = Table->FindRow<FP45_WeaponData>(RowName, TEXT("LookupWeapon"));
	if (!Row) { return false; }
	OutData = *Row;
	return true;
}

int32 UP45_WeaponDataTableHelper::CountRows(UDataTable* Table)
{
	return Table ? Table->GetRowMap().Num() : 0;
}
