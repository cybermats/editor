


#include "style/styletype.h"
#include "style/stylizer.h"

void ForegroundStyleType::accept(Stylizer *stylizer) const {
  stylizer->visit(this);
}

void BackgroundStyleType::accept(Stylizer *stylizer) const {
  stylizer->visit(this);
}

void BoldStyleType::accept(Stylizer *stylizer) const {
  stylizer->visit(this);
}
