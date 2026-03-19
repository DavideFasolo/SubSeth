#include "shared/UMultFamily.hpp"

using UMult6LBase = UMultLabeledModule<6>;

struct UMult6LPanel : SubPanel::LabeledPanel {
    UMult6LPanel() : SubPanel::LabeledPanel(12, "U·Mult·6L") {}
};

struct UMult6LWidget : UMultLabeledWidget<6, UMult6LBase, UMult6LPanel> {
    UMult6LWidget(UMult6LBase* module) : UMultLabeledWidget<6, UMult6LBase, UMult6LPanel>(module, 12) {}
};

Model* modelUMult6L = createModel<UMult6LBase, UMult6LWidget>("U-Mult-6L");
