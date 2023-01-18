#pragma once
#include "PackageObjectExplorerStructs.h"
#include "Debugging/SKismetDebugTreeView.h"

class FExportObjectInnerContext;


class SPackageObjectExplorer : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SPackageObjectExplorer ){}

	SLATE_END_ARGS()

	SPackageObjectExplorer();

	virtual ~SPackageObjectExplorer() override;


	
	void Construct( const FArguments& InArgs );

	void SetObjects(const TArray<UObject*>& InObjects );

	void SetAssetData(const TArray<FAssetData>& InAssetData );

protected:
	void RecursionLogInnerObjects(UObject* Object, TSharedPtr<FObjectExploreEntity> ParentEntity, const FExportObjectInnerContext* Context);

	TSharedRef<ITableRow> OnGenerateRow(TSharedPtr<FObjectExploreEntity> InTreeElement, const TSharedRef<STableViewBase>& OwnerTable);

	void OnGetChildren(TSharedPtr<FObjectExploreEntity> ObjectExploreEntity, TArray<TSharedPtr<FObjectExploreEntity>>& Children);

	void OnSelectionChanged(TSharedPtr<FObjectExploreEntity> ObjectExploreEntity, ESelectInfo::Type Arg);
	
private:
	TSharedPtr<STreeView<TSharedPtr<FObjectExploreEntity>>> TreeView;
	
	TArray<TSharedPtr<FObjectExploreEntity>> RootEntities;
	TSharedPtr<SKismetDebugTreeView> DebugTreeView;
};

