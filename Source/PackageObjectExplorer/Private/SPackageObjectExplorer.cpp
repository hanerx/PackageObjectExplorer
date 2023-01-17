#include "SPackageObjectExplorer.h"

#include "PackageObjectExplorer.h"
#include "Widgets/Text/SRichTextBlock.h"

#define LOCTEXT_NAMESPACE "PackageObjectExplorer"

SPackageObjectExplorer::SPackageObjectExplorer()
{
}

SPackageObjectExplorer::~SPackageObjectExplorer()
{
}

void SPackageObjectExplorer::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SNew(SBorder)
			[
				SAssignNew(TreeView, STreeView<TSharedPtr<FObjectExploreEntity>>)
				.TreeItemsSource(&RootEntities)
				.OnGenerateRow(this, &SPackageObjectExplorer::OnGenerateRow)
				.OnGetChildren(this, &SPackageObjectExplorer::OnGetChildren)
			]
		]
	];
}

void SPackageObjectExplorer::SetObjects(const TArray<UObject*>& InObjects)
{
	RootEntities.Empty();
	const FExportObjectInnerContext Context;
	for (const UObject* Object : InObjects)
	{
		const TSharedPtr<FObjectExploreEntity> Entity = MakeShareable(new FObjectExploreEntity());
		UObject* TargetObject=Object->GetPackage();
		RecursionLogInnerObjects(TargetObject, Entity, &Context);
		RootEntities.Add(Entity->Children[0]);
	}
}

void SPackageObjectExplorer::SetAssetData(const TArray<FAssetData>& InAssetData)
{
	TArray<UObject*> Objects;
	for (const FAssetData& AssetData : InAssetData)
	{
		Objects.Add(AssetData.GetPackage());
	}
	SetObjects(Objects);
}

void SPackageObjectExplorer::RecursionLogInnerObjects(UObject* Object, TSharedPtr<FObjectExploreEntity> ParentEntity,
                                                      const FExportObjectInnerContext* Context)
{
	const TSharedPtr<FObjectExploreEntity> Entity = MakeShareable(new FObjectExploreEntity());
	Entity->Object = Object;
	ParentEntity->Children.AddUnique(Entity);
	if (const auto InnerList = Context->GetObjectInners(Object)) //判断列表是否有效
	{
		//递归打印内部的物体
		for (UObject* Obj : *InnerList)
			RecursionLogInnerObjects(Obj, Entity, Context);
	}
}

TSharedRef<ITableRow> SPackageObjectExplorer::OnGenerateRow(TSharedPtr<FObjectExploreEntity> InTreeElement,
                                                            const TSharedRef<STableViewBase>& OwnerTable)
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("ObjectName"), FText::FromString(InTreeElement->Object?InTreeElement->Object->GetName():TEXT("Unknow")));
	Args.Add(TEXT("ClassName"),InTreeElement->Object?InTreeElement->Object->GetClass()->GetDisplayNameText():LOCTEXT("Unknow","Unknow"));
	Args.Add(TEXT("ClassRawName"),InTreeElement->Object?FText::FromString(InTreeElement->Object->GetClass()->GetName()):LOCTEXT("Unknow","Unknow"));
	const UClass* Native=nullptr;
	if(InTreeElement->Object)
	{
		for(Native=InTreeElement->Object->GetClass();!Native->IsNative();Native=Native->GetSuperClass());
	}
	Args.Add(TEXT("NativeCPPClassName"),Native?Native->GetDisplayNameText():LOCTEXT("Unknow","Unknow"));
	Args.Add(TEXT("NativeCPPClassRawName"),Native?FText::FromString(Native->GetName()):LOCTEXT("Unknow","Unknow"));
	return SNew(STableRow<TSharedPtr<FObjectExploreEntity>>, OwnerTable)
	[
		SNew(SHorizontalBox)
		// + SHorizontalBox::Slot()
		// [
		// 	SNew(STextBlock)
		// 	.Text(FText::Format(LOCTEXT("ObjectName","{ObjectName}"),Args))
		// ]
		// + SHorizontalBox::Slot()
		// [
		// 	SNew(STextBlock)
		// 	.Text(FText::Format(LOCTEXT("ClassName","( {ClassName} [{ClassRawName}] )"),Args))
		// 	.ColorAndOpacity(FLinearColor::Green)
		// ]
		// + SHorizontalBox::Slot()
		// [
		// 	SNew(STextBlock)
		// 	.Text(FText::Format(LOCTEXT("CPPName","-( {NativeCPPClassName} [{NativeCPPClassRawName}] )"),Args))
		// 	.ColorAndOpacity(FLinearColor::Yellow)
		// ]
		+SHorizontalBox::Slot()
		[
			SNew(SRichTextBlock)
			.Text(FText::Format(LOCTEXT("RichMessage","<RichTextBlock.TextHighlight>{ObjectName}</> ( <RichTextBlock.TextHighlight>{ClassName}</> [<RichTextBlock.Bold>{ClassRawName}</>] ) -<RichTextBlock.TextHighlight>{NativeCPPClassName}</> [<RichTextBlock.Bold>{NativeCPPClassRawName}</>]"),Args))
			.DecoratorStyleSet(&FEditorStyle::Get())
		]
	];
}

void SPackageObjectExplorer::OnGetChildren(TSharedPtr<FObjectExploreEntity> ObjectExploreEntity,
                                           TArray<TSharedPtr<FObjectExploreEntity>>& Children)
{
	Children = ObjectExploreEntity->Children;
}

#undef LOCTEXT_NAMESPACE