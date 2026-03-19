#include "shared/UMultFamily.hpp"

using UMult8LBase = UMultLabeledModule<8>;

struct UMult8LPanel : SubPanel::LabeledPanel {
    UMult8LPanel() : SubPanel::LabeledPanel(12, "U·Mult·8L") {}
};

struct UMult8LWidget : UMultLabeledWidget<8, UMult8LBase, UMult8LPanel> {
    UMult8LWidget(UMult8LBase* module) : UMultLabeledWidget<8, UMult8LBase, UMult8LPanel>(module, 12) {}
};

Model* modelUMult8L = createModel<UMult8LBase, UMult8LWidget>("U-Mult-8L");
