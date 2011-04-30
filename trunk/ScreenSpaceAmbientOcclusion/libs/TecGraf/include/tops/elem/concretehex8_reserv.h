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

#ifndef _TOP_CONCRETE_HEX8_RESERV_H
#define _TOP_CONCRETE_HEX8_RESERV_H

#include "../defines.h"

/**
    Linear hexahedron used as reservoir cell.

  @verbatim

                                   e9
                  v3               -->                v7
                  n3----------------------------------n7
                  /.                                  /| 
                 / .                                 / |
                /  .                                /  |
               /   .                               /   |
        e2 /  /    .         f5                   /    |
          v  /     . ^                     e6 /  /     |   ^
            /      . |e1                     v  /      |   | e5
           /       .                f2         /       |
          /        .                          /        |
        n2----------------------------------n6         |
        v2         .      -->               v6         |
         |         .      e10                |         |
         |   f0    .              e8         |    f1   |
         |         . /            -->        |         |
         |        n1 . . . . . . . . . . . . | . . . .n5
         |       .v1                         |        v5
   e3 |  |    ^ .       f3             e7 |  |       /
      v  | e0/ .                          v  |      /
         |    .                              |     /   ^
         |   .               f4              |    /   / e4
         |  .                                |   /
         | .                                 |  /
         |.                                  | /
        n0-----------------------------------n4
        v0                -->                v4
 y ^                      e11
   |  ^ x
   | /
   |/    
   * --->  
        z                 
    Orientation: n0 n1 n2 n3 n4 n5 n6 n7 
    Edges are locally oriented from its first vertex to the last one.
    fi: opposite face to vertex vi
  @endverbatim
*/
class TOPS_API TopConcreteHEX8Reservoir : public TopConcreteElement
{
private:
  static TopElementClass m_class;

  TopNode       m_nodes[8];
  TopElement    m_adj[6];
  unsigned char m_rev[6];
  
  TopConcreteHEX8Reservoir() {}
public:
  static TopElementClass* CLASS() { return &m_class; }
  static const char* CLASSNAME () { return "ConcreteHEX8Reservoir"; }
  virtual const char* ClassName () const { return CLASSNAME(); }

  static const TopElementTemplate     TEMPLATE();
  static const TopElementSizeTemplate SIZE_TEMPLATE();
  
  TopConcreteHEX8Reservoir(TopNode nodes[8]);
  ~TopConcreteHEX8Reservoir() {}

  void Init(TopNode* nodes);

  virtual bool Serialize(IOStream* io)
  {
    return TopConcreteElement::Serialize(io)      &&
           io->WriteUInts(8, (unsigned*) m_nodes) &&
           io->WriteUInts(6, (unsigned*) m_adj)   &&
           io->WriteBytes(6, m_rev);
  }

  virtual bool Deserialize(IOStream* io)
  {
    return TopConcreteElement::Deserialize(io)   &&
           io->ReadUInts(8, (unsigned*) m_nodes) &&
           io->ReadUInts(6, (unsigned*) m_adj)   &&
           io->ReadBytes(6, m_rev);
  }
  
  TopElementTemplate GetTemplate() { return TEMPLATE(); };
  TopElementSizeTemplate GetSizeTemplate() { return SIZE_TEMPLATE(); }

  const TopElementClass* GetClass() const { return &m_class; }
};

#endif
