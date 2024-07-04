#ifndef GRAVYENGINE_BVH_HPP
#define GRAVYENGINE_BVH_HPP

#include "BoundingBox.hpp"
#include "Vertex.hpp"
#include "../System/Mathf.hpp"
#include "../System/Numerics/Vector3.hpp"
#include <vector>
#include <cstdint>
#include <cstdlib>

namespace GravyEngine
{
    struct Triangle
    {
        Vector3 pA;
        Vector3 pB;
        Vector3 pC;
        Vector3 nA;
        Vector3 nB;
        Vector3 nC;

        Triangle() {}

        Triangle(const Vector3 &pA, const Vector3 &pB, const Vector3 &pC, const Vector3 &nA, const Vector3 &nB, const Vector3 &nC)
        {
            this->pA = pA;
            this->pB = pB;
            this->pC = pC;
            this->nA = nA;
            this->nB = nB;
            this->nC = nC;
        }
    };

    struct BVHNode
    {
        Vector3 BoundsMin;
        Vector3 BoundsMax;
        // Index of first child (if triangle count is negative) otherwise index of first triangle
        int StartIndex;
        int TriangleCount;

        BVHNode() {}

        BVHNode(const BoundingBox &bounds)
        {
            BoundsMin = bounds.GetMin();
            BoundsMax = bounds.GetMax();
            StartIndex = -1;
            TriangleCount = -1;
        }

        BVHNode(const BoundingBox &bounds, int startIndex, int triCount)
        {
            BoundsMin = bounds.GetMin();
            BoundsMax = bounds.GetMax();
            StartIndex = startIndex;
            TriangleCount = triCount;
        }

        Vector3 CalculateBoundsSize()
        {
            return BoundsMax - BoundsMin;
        }

        Vector3 CalculateBoundsCentre() 
        {
            return (BoundsMin + BoundsMax) * 0.5f;
        }
    };

    struct BVHTriangle
    {
        Vector3 Centre;
        Vector3 Min;
        Vector3 Max;
        int Index;

        BVHTriangle() {}

        BVHTriangle(const Vector3 &centre, const Vector3 &min, const Vector3 &max, int index)
        {
            Centre = centre;
            Min = min;
            Max = max;
            Index = index;
        }
    };


    class BVHNodeList
    {
    public:
        std::vector<BVHNode> Nodes;

        BVHNodeList()
        {
            Index = 0;
            Nodes.resize(256);
        }

        int Add(const BVHNode &node)
        {
            if (Index >= Nodes.size())
            {
                std::vector<BVHNode> nodesCopy;
                nodesCopy.resize(Nodes.size() * 2);

                for(size_t i = 0; i < Nodes.size(); i++)
                {
                    nodesCopy[i] = Nodes[i];
                }

                Nodes = nodesCopy;
            }

            int nodeIndex = Index;
            Nodes[Index++] = node;
            return nodeIndex;
        }

        int GetNodeCount()
        {
            return Index;
        };
    private:
        int Index;
    };

    struct BVHSplit
    {
        int axis;
        float pos;
        float cost;
    };



    class BVH
    {
    public:
        BVHNodeList allNodes;
        std::vector<Triangle> allTris;
        std::vector<BVHTriangle> AllTriangles;

        std::vector<Triangle> &GetTriangles()
        {
            return allTris;
        }

        BVH() {}

        BVH(const std::vector<Vertex> &verts, const std::vector<uint32_t> &indices)
        {
            Initialize(verts, indices);
        }

        void Initialize(const std::vector<Vertex> &verts, const std::vector<uint32_t> &indices)
        {
            allNodes.Nodes.resize(256);
            allTris.clear();
            AllTriangles.clear();

            // Construct BVH
            AllTriangles.resize(indices.size() / 3);
            BoundingBox bounds;

            for (size_t i = 0; i < indices.size(); i += 3)
            {
                Vector3 a = verts[indices[i + 0]].position;
                Vector3 b = verts[indices[i + 1]].position;
                Vector3 c = verts[indices[i + 2]].position;
                Vector3 centre = (a + b + c) / 3.0f;
                Vector3 max = Vector3f::Max(Vector3f::Max(a, b), c);
                Vector3 min = Vector3f::Min(Vector3f::Min(a, b), c);
                AllTriangles[i / 3] = BVHTriangle(centre, min, max, i);
                bounds.Grow(min, max);
            }

            allNodes.Add(BVHNode(bounds));
            Split(0, verts, 0, 8);
            //Split(0, verts, 0, 8);

            allTris.resize(AllTriangles.size());
            for (int i = 0; i < AllTriangles.size(); i++)
            {
                BVHTriangle buildTri = AllTriangles[i];
                Vector3 a = verts[indices[buildTri.Index + 0]].position;
                Vector3 b = verts[indices[buildTri.Index + 1]].position;
                Vector3 c = verts[indices[buildTri.Index + 2]].position;
                Vector3 norm_a = verts[indices[buildTri.Index + 0]].normal;
                Vector3 norm_b = verts[indices[buildTri.Index + 1]].normal;
                Vector3 norm_c = verts[indices[buildTri.Index + 2]].normal;
                allTris[i] = Triangle(a, b, c, norm_a, norm_b, norm_c);
            }
        }

        void Split(int parentIndex, const std::vector<Vertex> &verts, int triGlobalStart, int triNum, int depth = 0)
        {
            const int MaxDepth = 8;
            BVHNode &parent = allNodes.Nodes[parentIndex];
            Vector3 size = parent.CalculateBoundsSize();
            float parentCost = NodeCost(size, triNum);

            BVHSplit split = ChooseSplit(parent, triGlobalStart, triNum);
            int splitAxis = split.axis;
            float splitPos = split.pos;
            float cost = split.cost;

            if (cost < parentCost && depth < MaxDepth)
            {
                BoundingBox boundsLeft;
                BoundingBox boundsRight;
                int numOnLeft = 0;

                for (int i = triGlobalStart; i < triGlobalStart + triNum; i++)
                {
                    BVHTriangle tri = AllTriangles[i];
                    if (tri.Centre[splitAxis] < splitPos)
                    {
                        boundsLeft.Grow(tri.Min, tri.Max);

                        BVHTriangle swap = AllTriangles[triGlobalStart + numOnLeft];
                        AllTriangles[triGlobalStart + numOnLeft] = tri;
                        AllTriangles[i] = swap;
                        numOnLeft++;
                    }
                    else
                    {
                        boundsRight.Grow(tri.Min, tri.Max);
                    }
                }

                int numOnRight = triNum - numOnLeft;
                int triStartLeft = triGlobalStart + 0;
                int triStartRight = triGlobalStart + numOnLeft;

                // Split parent into two children
                int childIndexLeft = allNodes.Add(BVHNode(boundsLeft, triStartLeft, 0));
                int childIndexRight = allNodes.Add(BVHNode(boundsRight, triStartRight, 0));

                // Update parent
                parent.StartIndex = childIndexLeft;
                allNodes.Nodes[parentIndex] = parent;

                // Recursively split children
                Split(childIndexLeft, verts, triGlobalStart, numOnLeft, depth + 1);
                Split(childIndexRight, verts, triGlobalStart + numOnLeft, numOnRight, depth + 1);
            }
            else
            {
                // Parent is actually leaf, assign all triangles to it
                parent.StartIndex = triGlobalStart;
                parent.TriangleCount = triNum;
                allNodes.Nodes[parentIndex] = parent;
            }
        }

        BVHSplit ChooseSplit(const BVHNode &node, int start, int count)
        {
            if (count <= 1) 
            {
                BVHSplit s;
                s.axis = 0;
                s.pos = 0;
                s.cost = Mathf::Infinity;
                return s;
            }

            float bestSplitPos = 0;
            int bestSplitAxis = 0;
            const int numSplitTests = 5;

            float bestCost = Mathf::FloatMaxValue;

            // Estimate best split pos
            for (int axis = 0; axis < 3; axis++)
            {
                for (int i = 0; i < numSplitTests; i++)
                {
                    float splitT = (i + 1) / (numSplitTests + 1.0f);
                    float splitPos = Mathf::Lerp(node.BoundsMin[axis], node.BoundsMax[axis], splitT);
                    float cost = EvaluateSplit(axis, splitPos, start, count);
                    if (cost < bestCost)
                    {
                        bestCost = cost;
                        bestSplitPos = splitPos;
                        bestSplitAxis = axis;
                    }
                }
            }

            BVHSplit split;
            split.axis = bestSplitAxis;
            split.pos = bestSplitPos;
            split.cost = bestCost;

            return split;
        }

        float EvaluateSplit(int splitAxis, float splitPos, int start, int count)
        {
            BoundingBox boundsLeft;
            BoundingBox boundsRight;
            int numOnLeft = 0;
            int numOnRight = 0;

            for (int i = start; i < start + count; i++)
            {
                BVHTriangle tri = AllTriangles[i];
                if (tri.Centre[splitAxis] < splitPos)
                {
                    boundsLeft.Grow(tri.Min, tri.Max);
                    numOnLeft++;
                }
                else
                {
                    boundsRight.Grow(tri.Min, tri.Max);
                    numOnRight++;
                }
            }

            float costA = NodeCost(boundsLeft.GetSize(), numOnLeft);
            float costB = NodeCost(boundsRight.GetSize(), numOnRight);
            return costA + costB;
        }

        static float NodeCost(Vector3 size, int numTriangles)
        {
            float halfArea = size.x * size.y + size.x * size.z + size.y * size.z;
            return halfArea * numTriangles;
        }

        std::vector<BVHNode> &GetNodes()
        {
            return allNodes.Nodes;
        }
    };
};

#endif