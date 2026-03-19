#include "shared/UMultFamily.hpp"

using UMult12Base = UMultModule<12>;

struct UMult12Panel : SubPanel::LabeledPanel {
    UMult12Panel() : SubPanel::LabeledPanel(4, "U·Mult·12") {}
};

struct UMult12Widget : UMultCompactWidget<12, UMult12Base, UMult12Panel> {
    UMult12Widget(UMult12Base* module) : UMultCompactWidget<12, UMult12Base, UMult12Panel>(module, 4) {}
};

Model* modelUMult12 = createModel<UMult12Base, UMult12Widget>("U-Mult-12");
