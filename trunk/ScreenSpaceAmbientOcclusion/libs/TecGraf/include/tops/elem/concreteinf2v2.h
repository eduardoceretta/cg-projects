/**
  TopS - Topological data structure for mesh representation.
  
  TopS is a software developed by Tecgraf/PUC-Rio & UIUC.
  It is requested that the TopS users provide the appropriate
  credits and references to the work.
  
  References:
  1 -W. Celes, G.H. Paulino, R. Espinha, "A compact adjacency-based
  topological data structure for finite element mesh representation",
  IJNME, 2005 (in press)
  2 - W. Celes, G.H. Paulino, R. Espinha, "Efficient Handling of Implicit
  Entities in Reduced Mesh Representations", JCISE, Special Issue on
  Mesh-Based Geometric Data Processing, 2005 (in press)
  
  Aug 2005
*/

#ifndef _TOP_CONCRETE_INF2V2_H
#define _TOP_CONCRETE_INF2V2_H

#include "../defines.h"

/**
    Linear 2D infinite element.

  @verbatim
           
         v2
         n2 .
          |   .
          |     .  ^
  f2 e2 | |       . \ e1 f1 
        v |         .
          |           .
          |             .
         n0---------------n1
         v0    e0 ->      v1
               f0
  
    Orientation: n0 n1 n2
    Edges are locally oriented from its first vertex to the last one.
  @endverbatim
*/
class TOPS_API TopConcreteINF2V2 : public TopConcreteElement
{
private:
  static TopElementClass m_class;

  TopNode       m_nodes[3];
  TopElement    m_adj[3];
  unsigned char m_rev[3];
  
  TopConcreteINF2V2() {}

public:
  static TopElementClass* CLASS() { return &m_class; }
  static const char* CLASSNAME () { return "ConcreteINF2V2"; }
  virtual const char* ClassName () const { return CLASSNAME(); }

  static const TopElementTemplate     TEMPLATE();
  static const TopElementSizeTemplate SIZE_TEMPLATE();

  TopConcreteINF2V2(TopNode nodes[3]);
  ~TopConcreteINF2V2() {}

  void Init(TopNode* nodes);

  virtual bool Serialize(IOStream* io)
  {
    return TopConcreteElement::Serialize(io)      &&
           io->WriteUInts(3, (unsigned*) m_nodes) &&
           io->WriteUInts(3, (unsigned*) m_adj)   &&
           io->WriteBytes(3, m_rev);
  }

  virtual bool Deserialize(IOStream* io)
  {
    return TopConcreteElement::Deserialize(io)   &&
           io->ReadUInts(3, (unsigned*) m_nodes) &&
           io->ReadUInts(3, (unsigned*) m_adj)   &&
           io->ReadBytes(3, m_rev);
  }
  
  TopElementTemplate GetTemplate() { return TEMPLATE(); };
  TopElementSizeTemplate GetSizeTemplate() { return SIZE_TEMPLATE(); }  

  const TopElementClass* GetClass() const { return &m_class; }
};

#endif
