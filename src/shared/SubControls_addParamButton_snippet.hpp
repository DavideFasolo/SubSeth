// Snippet opzionale per src/shared/SubControls.hpp
// Ti permette di aggiungere i pulsanti come fai già con i connettori.

template <typename TButton>
static inline TButton* addParamButton(
	ModuleWidget* moduleWidget,
	SubPanel::LabeledPanel* panel,
	Module* module,
	Vec pos,
	int paramId,
	const char* labelText = nullptr,
	SubPanel::LabelPlacement placement = {}
) {
	TButton* button = createParamCentered<TButton>(pos, module, paramId);
	moduleWidget->addParam(button);
	if (panel && labelText && labelText[0]) {
		panel->addObjLabel(button, labelText, placement);
	}
	return button;
}
