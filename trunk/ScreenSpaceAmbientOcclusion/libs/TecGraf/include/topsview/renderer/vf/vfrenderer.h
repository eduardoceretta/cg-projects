#ifndef TPV_VECTORFIELD_RENDERER_H
#define TPV_VECTORFIELD_RENDERER_H

//----------//
// Includes //
//----------//

#include <vfl/linerenderer.h>
#include <topsview/geometry/vf/fieldline.h>

//----------------------//
// Forward Declarations //
//----------------------//

//----------------------------------------------//
//  Class Definition: TpvAbstractPropertyGetter //
//----------------------------------------------//

// Please see class VflFieldLineRenderer for methods information
/*
class TpvFieldLineRenderer : public VflFieldLineRenderer
{
public:

  TpvFieldLineRenderer(unsigned int colorModel = COLOR_TANGENTS, unsigned int lightModel = LIGHT_NONE)
  : VflFieldLineRenderer(colorModel, lightModel)
  {
  }

  virtual ~TpvFieldLineRenderer(void)
  {
  }

  void Render (TpvFieldLineList *fieldLineList)
  {
    for (TpvFieldLineList::Iterator it = fieldLineList->Begin(); it.Valid(); ++it)
    {
      TpvFieldLine *fieldLine = it.Self();
      if (fieldLine) Render(fieldLine);
    }
  }

  void Render (TpvFieldLine *fieldLine)
  {
    VflFieldLineRenderer::Render((VflFieldLine*)fieldLine);
  }

};
*/
#endif // TPV_VECTORFIELD_RENDERER_H
