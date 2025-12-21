#pragma once

/// \file
/// Geometry primitive utilities

#include "../../Primitives/interface/DataBlob.h"
#include "../../Primitives/interface/FlagEnum.h"

namespace Diligent
{

/// Geometry primitive vertex flags.
enum GEOMETRY_PRIMITIVE_VERTEX_FLAGS : UInt32
{
    /// No flags.
    GEOMETRY_PRIMITIVE_VERTEX_FLAG_NONE = 0u,

    /// The geometry primitive vertex contains position.
    GEOMETRY_PRIMITIVE_VERTEX_FLAG_POSITION = 1u << 0u,

    /// The geometry primitive vertex contains normal.
    GEOMETRY_PRIMITIVE_VERTEX_FLAG_NORMAL = 1u << 1u,

    /// The geometry primitive vertex contains texture coordinates.
    GEOMETRY_PRIMITIVE_VERTEX_FLAG_TEXCOORD = 1u << 2u,

    /// Special value that indicates the last vertex flag.
    GEOMETRY_PRIMITIVE_VERTEX_FLAG_LAST = GEOMETRY_PRIMITIVE_VERTEX_FLAG_TEXCOORD,

    /// Geometry primitive contains all vertex components.
    GEOMETRY_PRIMITIVE_VERTEX_FLAG_ALL = GEOMETRY_PRIMITIVE_VERTEX_FLAG_POSITION | GEOMETRY_PRIMITIVE_VERTEX_FLAG_NORMAL | GEOMETRY_PRIMITIVE_VERTEX_FLAG_TEXCOORD,

    /// Geometry primitive vertex contains position and normal.
    GEOMETRY_PRIMITIVE_VERTEX_FLAG_POS_NORM = GEOMETRY_PRIMITIVE_VERTEX_FLAG_POSITION | GEOMETRY_PRIMITIVE_VERTEX_FLAG_NORMAL,

    /// Geometry primitive vertex contains position and texture coordinates.
    GEOMETRY_PRIMITIVE_VERTEX_FLAG_POS_TEX = GEOMETRY_PRIMITIVE_VERTEX_FLAG_POSITION | GEOMETRY_PRIMITIVE_VERTEX_FLAG_TEXCOORD,
};

DEFINE_FLAG_ENUM_OPERATORS(GEOMETRY_PRIMITIVE_VERTEX_FLAGS)

/// Geometry primitive types.
enum GEOMETRY_PRIMITIVE_TYPE : UInt32
{
    /// Geometry primitive type is undefined.
    GEOMETRY_PRIMITIVE_TYPE_UNDEFINED = 0u,

    /// Cube geometry primitive type.
    GEOMETRY_PRIMITIVE_TYPE_CUBE,

    /// Sphere geometry primitive type.
    GEOMETRY_PRIMITIVE_TYPE_SPHERE,

    /// Geometry primitive type count.
    GEOMETRY_PRIMITIVE_TYPE_COUNT
};

/// Geometry primitive attributes.
struct GeometryPrimitiveAttributes
{
    /// The geometry primitive type, see Diligent::GEOMETRY_PRIMITIVE_TYPE.
    GEOMETRY_PRIMITIVE_TYPE Type DEFAULT_INITIALIZER(GEOMETRY_PRIMITIVE_TYPE_UNDEFINED);

    /// Vertex flags that specify which vertex components to include in the output vertices,
    /// see Diligent::GEOMETRY_PRIMITIVE_VERTEX_FLAGS.
    GEOMETRY_PRIMITIVE_VERTEX_FLAGS VertexFlags DEFAULT_INITIALIZER(GEOMETRY_PRIMITIVE_VERTEX_FLAG_ALL);

    /// The number of subdivisions.

    /// This parameter defines the fidelity of the geometry primitive.
    /// For example, for a cube geometry primitive, the cube faces are subdivided
    /// into `Subdivision x Subdivision` quads, producing `(Subdivision + 1)^2` vertices
    /// per face.
    UInt32 NumSubdivisions = 0;

    GeometryPrimitiveAttributes() noexcept = default;

    explicit GeometryPrimitiveAttributes(GEOMETRY_PRIMITIVE_TYPE         _Type,
                                         GEOMETRY_PRIMITIVE_VERTEX_FLAGS _VertexFlags    = GEOMETRY_PRIMITIVE_VERTEX_FLAG_ALL,
                                         UInt32                          _NumSubdivision = 1) noexcept :
        Type{_Type},
        VertexFlags{_VertexFlags},
        NumSubdivisions{_NumSubdivision}
    {}
};
typedef struct GeometryPrimitiveAttributes GeometryPrimitiveAttributes;

/// Cube geometry primitive attributes.
// clang-format off
struct CubeGeometryPrimitiveAttributes : public GeometryPrimitiveAttributes {

    /// The size of the cube.

    /// The cube is centered at (0, 0, 0) and has the size of Size x Size x Size.
    /// If the cube size is 1, the coordinates of the cube vertices are in the range [-0.5, 0.5].
    float Size = 1.f;

	explicit CubeGeometryPrimitiveAttributes(float                           _Size           = 1,
                                             GEOMETRY_PRIMITIVE_VERTEX_FLAGS _VertexFlags    = GEOMETRY_PRIMITIVE_VERTEX_FLAG_ALL,
                                             UInt32                          _NumSubdivision = 1) noexcept :
        GeometryPrimitiveAttributes{GEOMETRY_PRIMITIVE_TYPE_CUBE, _VertexFlags, _NumSubdivision},
        Size{_Size}
    {}
};
// clang-format on


/// Sphere geometry primitive attributes.
// clang-format off
struct SphereGeometryPrimitiveAttributes : public GeometryPrimitiveAttributes {

    /// Sphere radius.
    float Radius = 1.f;

	explicit SphereGeometryPrimitiveAttributes(float                           _Radius         = 1,
                                               GEOMETRY_PRIMITIVE_VERTEX_FLAGS _VertexFlags    = GEOMETRY_PRIMITIVE_VERTEX_FLAG_ALL,
                                               UInt32                          _NumSubdivision = 1) noexcept :
        GeometryPrimitiveAttributes{GEOMETRY_PRIMITIVE_TYPE_SPHERE, _VertexFlags, _NumSubdivision},
        Radius{_Radius}
    {}
};
// clang-format on

/// Geometry primitive info.
struct GeometryPrimitiveInfo
{
    /// The number of vertices.
    UInt32 NumVertices = 0;

    /// The number of indices.
    UInt32 NumIndices = 0;

    /// The size of the vertex in bytes.
    UInt32 VertexSize = 0;
};
typedef struct GeometryPrimitiveInfo GeometryPrimitiveInfo;

/// Returns the size of the geometry primitive vertex in bytes.
UInt32 GetGeometryPrimitiveVertexSize(GEOMETRY_PRIMITIVE_VERTEX_FLAGS VertexFlags);

/// Creates a geometry primitive
///
/// \param [in]  Attribs    - Geometry primitive attributes, see Diligent::GeometryPrimitiveAttributes.
/// \param [out] ppVertices - Address of the memory location where the pointer to the output vertex data blob will be stored.
///                           The vertex components are stored as interleaved floating-point values.
///                           For example, if VertexFlags = Diligent::GEOMETRY_PRIMITIVE_VERTEX_FLAG_POS_NORM, the vertex data will
///                           be stored as follows:
///                             P0, N0, P1, N1, ..., Pn, Nn.
/// \param [out] ppIndices  - Address of the memory location where the pointer to the output index data blob will be stored.
///                           Index data is stored as 32-bit unsigned integers representing the triangle list.
/// \param [out] pInfo      - A pointer to the structure that will receive information about the created geometry primitive.
///                           See Diligent::GeometryPrimitiveInfo.
void CreateGeometryPrimitive(const GeometryPrimitiveAttributes& Attribs, IDataBlob** ppVertices, IDataBlob** ppIndices, GeometryPrimitiveInfo* pInfo = nullptr);


} // namespace Diligent
