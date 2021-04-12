// Copyright 2020 Sabre Dart Studios


#include "zSpace/UI/LoginUserWidgetBase.h"

#include "Components/EditableTextBox.h"

void ULoginUserWidgetBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(txtEmail) && IsValid(txtPassword))
	{
		if (!txtEmail->OnTextCommitted.IsAlreadyBound(this, &ULoginUserWidgetBase::OnTextCommitted))
		{
			txtEmail->OnTextCommitted.AddDynamic(this, &ULoginUserWidgetBase::OnTextCommitted);
		}
		if (!txtPassword->OnTextCommitted.IsAlreadyBound(this, &ULoginUserWidgetBase::OnTextCommitted))
		{
			txtPassword->OnTextCommitted.AddDynamic(this, &ULoginUserWidgetBase::OnTextCommitted);
		}
	}
}

void ULoginUserWidgetBase::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		OnPressedEnter();
	}
}
