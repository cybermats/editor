//
// Created by mats on 11/11/18.
//

#include "style/style.h"
#include "style/stylizer.h"

void ForegroundStyle::accept(Stylizer *stylizer) const {
  stylizer->visit(*this);
}

void BackgroundStyle::accept(Stylizer *stylizer) const {
  stylizer->visit(*this);
}

void BoldStyle::accept(Stylizer *stylizer) const {
  stylizer->visit(*this);
}
