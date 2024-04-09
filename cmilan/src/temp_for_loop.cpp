else if(match(T_FOR)) {
		mustBe(T_LPAREN);
    mustSee(T_IDENTIFIER);
    // запомним адрес переменной
    int varAddress = findOrAddVariable(scanner_->getStringValue());
    assignment();
    mustBe(T_SEMICOLON);
		//запоминаем адрес начала проверки условия.
		int conditionAddress = codegen_->getCurrentAddress();
		relation();
    mustBe(T_SEMICOLON);
    //резервируем место под инструкцию условного перехода для выхода из цикла.
    int jumpNoAddress = codegen_->reserve();
    //резервируем место под инструкцию условного перехода для перехода к телу цикла.
    int jumpEqAddress = codegen_->reserve();
    //запоминаем адрес выражения.
    int expressionAddress = codegen_->getCurrentAddress();
    expression();
    //обновим значение переменной
    codegen_->emit(STORE, varAddress);
    //переходим по адресу проверки условия
    codegen_->emit(JUMP, conditionAddress);
    mustBe(T_RPAREN);
    //заполняем адрес условного перехода к телу цикла.
    codegen_->emitAt(jumpEqAddress, JUMP, codegen_->getCurrentAddress());
    mustBe(T_DO);
		statementList();
		mustBe(T_OD);
    //переходим по адресу выражения
    codegen_->emit(JUMP, expressionAddress);
		//заполняем зарезервированный адрес инструкцией условного перехода на следующий за циклом оператор.
		codegen_->emitAt(jumpNoAddress, JUMP_NO, codegen_->getCurrentAddress());
	}