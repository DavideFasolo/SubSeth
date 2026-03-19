#include "shared/UMultFamily.hpp"

using UMult4Base = UMultModule<4>;

struct UMult4Panel : SubPanel::LabeledPanel {
    UMult4Panel() : SubPanel::LabeledPanel(4, "U·Mult·4") {}
};

struct UMult4Widget : UMultCompactWidget<4, UMult4Base, UMult4Panel> {
    UMult4Widget(UMult4Base* module) : UMultCompactWidget<4, UMult4Base, UMult4Panel>(module, 4) {}
};

Model* modelUMult4 = createModel<UMult4Base, UMult4Widget>("U-Mult-4");
