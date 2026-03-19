#include "shared/UMultFamily.hpp"

using UMult4LBase = UMultLabeledModule<4>;

struct UMult4LPanel : SubPanel::LabeledPanel {
    UMult4LPanel() : SubPanel::LabeledPanel(12, "U·Mult·4L") {}
};

struct UMult4LWidget : UMultLabeledWidget<4, UMult4LBase, UMult4LPanel> {
    UMult4LWidget(UMult4LBase* module) : UMultLabeledWidget<4, UMult4LBase, UMult4LPanel>(module, 12) {}
};

Model* modelUMult4L = createModel<UMult4LBase, UMult4LWidget>("U-Mult-4L");
