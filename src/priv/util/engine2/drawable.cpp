#include "pch.h"

//
//
// -----------------------------------------------------------------------------
GN::engine2::Drawable::Drawable()
    : mKernel(0), mParamSet(0), mBinding(0), mContext(0)
{
}

//
//
// -----------------------------------------------------------------------------
GN::engine2::Drawable::~Drawable()
{
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::Drawable::clear()
{
    mKernel = 0;
    mParamSet = 0;
    mBinding = 0;
    mContext = 0;
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine2::Drawable::loadFromXmlNode(
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
bool GN::engine2::Drawable::loadFromXmlFile(
    RenderEngine  & ,
    const StrA    & )
{
    GN_UNIMPL();
    return false;
}
