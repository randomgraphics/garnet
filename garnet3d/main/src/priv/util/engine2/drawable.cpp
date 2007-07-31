#include "pch.h"

//
//
// -----------------------------------------------------------------------------
GN::engine::Drawable::Drawable()
    : mKernel(0), mParamSet(0), mBinding(0), mContext(0)
{
}

//
//
// -----------------------------------------------------------------------------
GN::engine::Drawable::~Drawable()
{
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::Drawable::clear()
{
    mKernel = 0;
    mParamSet = 0;
    mBinding = 0;
    mContext = 0;
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::Drawable::loadFromXmlNode(
    RenderEngine  & ,
    const XmlNode & ,
    const StrA    &  )
{
    GN_UNIMPL();
    return false;
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::Drawable::loadFromXmlFile(
    RenderEngine  & ,
    const StrA    & )
{
    GN_UNIMPL();
    return false;
}
