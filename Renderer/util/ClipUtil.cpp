# include "ClipUtil.hpp"

void ClipUtil::_doClppingInCvvAgainstLeftPlane(vector<Triangle> &triList)
{
    for (auto iter = triList.begin(); iter != triList.end(); /*do nothing*/)
    {
        Triangle &tri = *iter;

        vector<VertexOut> vertice = {tri.v1, tri.v2, tri.v3};
        vector<bool> bList = _getbOutListForLeftPlane(tri);

        int outNum = 0;
        for (int i = 0; i < bList.size(); ++i)
        {
            bool out = bList.at(i);
            outNum = outNum + (out ? 1 : 0);
        }
        if (outNum == 0)
        {
            // 如果没有点在外部，不处理
            ++iter;
            continue;
        }
        else if (outNum == 2)
        {
            // 如果有两个顶点在外部，构造一个新三角形（修改旧三角形�?
            // 储存外侧顶点的索�?
            vector<int> indiceOut;
            // 内侧顶点的索�?
            int inIdx = 0;
            // 找到在外侧的点的索引
            for (int i = 0; i < bList.size(); ++i)
            {
                if (bList.at(i))
                {
                    indiceOut.push_back(i);
                }
                else
                {
                    inIdx = i;
                }
            }
            VertexOut &vertIn = vertice.at(inIdx);
            // 遍历外侧顶点，插值生成新的顶�?
            for (int i = 0; i < indiceOut.size(); ++i)
            {
                int index = indiceOut.at(i);
                VertexOut &vertOut = vertice.at(index);
                // ax + (cx - ax) * factor = newX
                // aw + (cw - aw) * factor = newW
                // newX = - newW
                double factor = _getFactorForLeftPlane(vertIn, vertOut);
                VertexOut vertNew = vertIn.interpolate(vertOut, factor);
                vertice[index] = vertNew;
            }
            tri.v1 = vertice[0];
            tri.v2 = vertice[1];
            tri.v3 = vertice[2];
            ++iter;
        }
        else if (outNum == 1)
        {
            // 如果有一个顶点在外部 ,构造一个梯形（修改旧三角形+添加一个新三角形）
            // 储存内侧顶点的索�?
            vector<int> indiceIn;
            // 外侧顶点的索�?
            int outIdx = 0;
            // 找到内侧的点的索�?
            for (int i = 0; i < bList.size(); ++i)
            {
                if (!bList.at(i))
                {
                    indiceIn.push_back(i);
                }
                else
                {
                    outIdx = i;
                }
            }
            VertexOut &vertOut = vertice.at(outIdx);
            // 遍历内侧顶点，插值生成新的顶点，并添加三角形
            vector<VertexOut> vertNewList;
            for (int i = 0; i < indiceIn.size(); ++i)
            {
                // 生成对应新顶�?
                int index = indiceIn.at(i);
                VertexOut vertIn = vertice.at(index);
                // ax + (cx - ax) * factor = newX
                // aw + (cw - aw) * factor = newW
                // newX = - newW
                // double factor = (vertIn.pos.x + vertIn.pos.w) / (vertIn.pos.x - vertOut.pos.x + vertIn.pos.w - vertOut.pos.w);
                double factor = _getFactorForLeftPlane(vertIn, vertOut);
                VertexOut vertNew = vertIn.interpolateEarly(vertOut, factor);
                vertNewList.push_back(vertIn);
                vertNewList.push_back(vertNew);
            }
            tri.v1 = vertNewList[0];
            tri.v2 = vertNewList[1];
            tri.v3 = vertNewList[2];

            Triangle triNew(vertNewList[1], vertNewList[2], vertNewList[3]);
            iter = triList.insert(iter + 1, triNew);
            ++iter;
        }
        else if (outNum == 3)
        {
            // 如果有三个顶点在外部，整体剔�?
            iter = triList.erase(iter);
        }
    }
}

void ClipUtil::_doClppingInCvvAgainstRightPlane(vector<Triangle> &triList)
{
    for (auto iter = triList.begin(); iter != triList.end(); /*do nothing*/)
    {
        Triangle &tri = *iter;
        
        vector<VertexOut> vertice = {tri.v1, tri.v2, tri.v3};
        vector<bool> bList = _getbOutListForRightPlane(tri);

        int outNum = 0;
        for (int i = 0; i < bList.size(); ++i)
        {
            bool out = bList.at(i);
            outNum = outNum + (out ? 1 : 0);
        }
        if (outNum == 0)
        {
            // 如果没有点在外部，不处理
            ++iter;
            continue;
        }
        else if (outNum == 2)
        {
            // 如果有两个顶点在外部，构造一个新三角形（修改旧三角形�?
            // 储存外侧顶点的索�?
            vector<int> indiceOut;
            // 内侧顶点的索�?
            int inIdx = 0;
            // 找到在外侧的点的索引
            for (int i = 0; i < bList.size(); ++i)
            {
                if (bList.at(i))
                {
                    indiceOut.push_back(i);
                }
                else
                {
                    inIdx = i;
                }
            }
            VertexOut &vertIn = vertice.at(inIdx);
            // 遍历外侧顶点，插值生成新的顶�?
            for (int i = 0; i < indiceOut.size(); ++i)
            {
                int index = indiceOut.at(i);
                VertexOut &vertOut = vertice.at(index);
                // ax + (cx - ax) * factor = newX
                // aw + (cw - aw) * factor = newW
                // newX = newW
                double factor = _getFactorForRightPlane(vertIn, vertOut);
                VertexOut vertNew = vertIn.interpolate(vertOut, factor);
                vertice[index] = vertNew;
            }
            tri.v1 = vertice[0];
            tri.v2 = vertice[1];
            tri.v3 = vertice[2];
            ++iter;
        }
        else if (outNum == 1)
        {
            // 如果有一个顶点在外部 ,构造一个梯形（修改旧三角形+添加一个新三角形）
            // 储存内侧顶点的索�?
            vector<int> indiceIn;
            // 外侧顶点的索�?
            int outIdx = 0;
            // 找到内侧的点的索�?
            for (int i = 0; i < bList.size(); ++i)
            {
                if (!bList.at(i))
                {
                    indiceIn.push_back(i);
                }
                else
                {
                    outIdx = i;
                }
            }
            VertexOut &vertOut = vertice.at(outIdx);
            // 遍历内侧顶点，插值生成新的顶点，并添加三角形
            vector<VertexOut> vertNewList;
            for (int i = 0; i < indiceIn.size(); ++i)
            {
                // 生成对应新顶�?
                int index = indiceIn.at(i);
                VertexOut vertIn = vertice.at(index);
                // ax + (cx - ax) * factor = newX
                // aw + (cw - aw) * factor = newW
                // newX = newW
                // double factor = (vertIn.pos.x - vertIn.pos.w) / (vertIn.pos.x - vertOut.pos.x + vertOut.pos.w - vertIn.pos.w);
                double factor = _getFactorForRightPlane(vertIn, vertOut);
                VertexOut vertNew = vertIn.interpolateEarly(vertOut, factor);
                vertNewList.push_back(vertIn);
                vertNewList.push_back(vertNew);
            }
            tri.v1 = vertNewList[0];
            tri.v2 = vertNewList[1];
            tri.v3 = vertNewList[2];

            Triangle triNew(vertNewList[1], vertNewList[2], vertNewList[3]);
            iter = triList.insert(iter + 1, triNew);
            ++iter;
        }
        else if (outNum == 3)
        {
            // 如果有三个顶点在外部，整体剔�?
            iter = triList.erase(iter);
        }
    }
}

void ClipUtil::_doClppingInCvvAgainstTopPlane(vector<Triangle> &triList)
{
    for (auto iter = triList.begin(); iter != triList.end(); /*do nothing*/)
    {
        Triangle &tri = *iter;

        vector<VertexOut> vertice = {tri.v1, tri.v2, tri.v3};
        vector<bool> bList = _getbOutListForTopPlane(tri);

        int outNum = 0;
        for (int i = 0; i < bList.size(); ++i)
        {
            bool out = bList.at(i);
            outNum = outNum + (out ? 1 : 0);
        }
        if (outNum == 0)
        {
            // 如果没有点在外部，不处理
            ++iter;
            continue;
        }
        else if (outNum == 2)
        {
            // 如果有两个顶点在外部，构造一个新三角形（修改旧三角形�?
            // 储存外侧顶点的索�?
            vector<int> indiceOut;
            // 内侧顶点的索�?
            int inIdx = 0;
            // 找到在外侧的点的索引
            for (int i = 0; i < bList.size(); ++i)
            {
                if (bList.at(i))
                {
                    indiceOut.push_back(i);
                }
                else
                {
                    inIdx = i;
                }
            }
            VertexOut &vertIn = vertice.at(inIdx);
            // 遍历外侧顶点，插值生成新的顶�?
            for (int i = 0; i < indiceOut.size(); ++i)
            {
                int index = indiceOut.at(i);
                VertexOut &vertOut = vertice.at(index);
                // ay + (cy - ay) * factor = newY
                // aw + (cw - aw) * factor = newW
                // newY = newW
                double factor = _getFactorForTopPlane(vertIn, vertOut);
                VertexOut vertNew = vertIn.interpolate(vertOut, factor);
                vertice[index] = vertNew;
            }
            tri.v1 = vertice[0];
            tri.v2 = vertice[1];
            tri.v3 = vertice[2];
            ++iter;
        }
        else if (outNum == 1)
        {
            // 如果有一个顶点在外部 ,构造一个梯形（修改旧三角形+添加一个新三角形）
            // 储存内侧顶点的索�?
            vector<int> indiceIn;
            // 外侧顶点的索�?
            int outIdx = 0;
            // 找到内侧的点的索�?
            for (int i = 0; i < bList.size(); ++i)
            {
                if (!bList.at(i))
                {
                    indiceIn.push_back(i);
                }
                else
                {
                    outIdx = i;
                }
            }
            VertexOut &vertOut = vertice.at(outIdx);
            // 遍历内侧顶点，插值生成新的顶点，并添加三角形
            vector<VertexOut> vertNewList;
            for (int i = 0; i < indiceIn.size(); ++i)
            {
                // 生成对应新顶�?
                int index = indiceIn.at(i);
                VertexOut vertIn = vertice.at(index);
                // ay + (cy - ay) * factor = newY
                // aw + (cw - aw) * factor = newW
                // newY = newW
                // double factor = (vertIn.pos.y - vertIn.pos.w) / (vertIn.pos.y - vertOut.pos.y + vertOut.pos.w - vertIn.pos.w);
                double factor = _getFactorForTopPlane(vertIn, vertOut);
                VertexOut vertNew = vertIn.interpolateEarly(vertOut, factor);
                vertNewList.push_back(vertIn);
                vertNewList.push_back(vertNew);
            }
            tri.v1 = vertNewList[0];
            tri.v2 = vertNewList[1];
            tri.v3 = vertNewList[2];

            Triangle triNew(vertNewList[1], vertNewList[2], vertNewList[3]);
            iter = triList.insert(iter + 1, triNew);
            ++iter;
        }
        else if (outNum == 3)
        {
            // 如果有三个顶点在外部，整体剔�?
            iter = triList.erase(iter);
        }
    }
}

void ClipUtil::_doClppingInCvvAgainstBottomPlane(vector<Triangle> &triList)
{
    for (auto iter = triList.begin(); iter != triList.end(); /*do nothing*/)
    {
        Triangle &tri = *iter;
        
        vector<VertexOut> vertice = {tri.v1, tri.v2, tri.v3};
        vector<bool> bList = _getbOutListForBottomPlane(tri);

        int outNum = 0;
        for (int i = 0; i < bList.size(); ++i)
        {
            bool out = bList.at(i);
            outNum = outNum + (out ? 1 : 0);
        }
        if (outNum == 0)
        {
            // 如果没有点在外部，不处理
            ++iter;
            continue;
        }
        else if (outNum == 2)
        {
            // 如果有两个顶点在外部，构造一个新三角形（修改旧三角形�?
            // 储存外侧顶点的索�?
            vector<int> indiceOut;
            // 内侧顶点的索�?
            int inIdx = 0;
            // 找到在外侧的点的索引
            for (int i = 0; i < bList.size(); ++i)
            {
                if (bList.at(i))
                {
                    indiceOut.push_back(i);
                }
                else
                {
                    inIdx = i;
                }
            }
            VertexOut &vertIn = vertice.at(inIdx);
            // 遍历外侧顶点，插值生成新的顶�?
            for (int i = 0; i < indiceOut.size(); ++i)
            {
                int index = indiceOut.at(i);
                VertexOut &vertOut = vertice.at(index);
                // ay + (cy - ay) * factor = newY
                // aw + (cw - aw) * factor = newW
                // newY = - newW
                double factor = _getFactorForBottomPlane(vertIn, vertOut);
                VertexOut vertNew = vertIn.interpolate(vertOut, factor);
                vertice[index] = vertNew;
            }
            tri.v1 = vertice[0];
            tri.v2 = vertice[1];
            tri.v3 = vertice[2];
            ++iter;
        }
        else if (outNum == 1)
        {
            // 如果有一个顶点在外部 ,构造一个梯形（修改旧三角形+添加一个新三角形）
            // 储存内侧顶点的索�?
            vector<int> indiceIn;
            // 外侧顶点的索�?
            int outIdx = 0;
            // 找到内侧的点的索�?
            for (int i = 0; i < bList.size(); ++i)
            {
                if (!bList.at(i))
                {
                    indiceIn.push_back(i);
                }
                else
                {
                    outIdx = i;
                }
            }
            VertexOut &vertOut = vertice.at(outIdx);
            // 遍历内侧顶点，插值生成新的顶点，并添加三角形
            vector<VertexOut> vertNewList;
            for (int i = 0; i < indiceIn.size(); ++i)
            {
                // 生成对应新顶�?
                int index = indiceIn.at(i);
                VertexOut vertIn = vertice.at(index);
                // ay + (cy - ay) * factor = newY
                // aw + (cw - aw) * factor = newW
                // newY = - newW
                // double factor = (vertIn.pos.y + vertIn.pos.w) / (vertIn.pos.y - vertOut.pos.y + vertIn.pos.w - vertOut.pos.w);
                double factor = _getFactorForBottomPlane(vertIn, vertOut);

                VertexOut vertNew = vertIn.interpolateEarly(vertOut, factor);
                vertNewList.push_back(vertIn);
                vertNewList.push_back(vertNew);
            }
            tri.v1 = vertNewList[0];
            tri.v2 = vertNewList[1];
            tri.v3 = vertNewList[2];

            Triangle triNew(vertNewList[1], vertNewList[2], vertNewList[3]);
            iter = triList.insert(iter + 1, triNew);
            ++iter;
        }
        else if (outNum == 3)
        {
            // 如果有三个顶点在外部，整体剔�?
            iter = triList.erase(iter);
        }
    }
}

void ClipUtil::_doClppingInCvvAgainstFarPlane(vector<Triangle> &triList)
{
    for (auto iter = triList.begin(); iter != triList.end(); /*do nothing*/)
    {
        Triangle &tri = *iter;
        
        vector<VertexOut> vertice = {tri.v1, tri.v2, tri.v3};
        vector<bool> bList = _getbOutListForFarPlane(tri);

        int outNum = 0;
        for (int i = 0; i < bList.size(); ++i)
        {
            bool out = bList.at(i);
            outNum = outNum + (out ? 1 : 0);
        }
        if (outNum == 0)
        {
            // 如果没有点在外部，不处理
            ++iter;
            continue;
        }
        else if (outNum == 2)
        {
            // 如果有两个顶点在外部，构造一个新三角形（修改旧三角形�?
            // 储存外侧顶点的索�?
            vector<int> indiceOut;
            // 内侧顶点的索�?
            int inIdx = 0;
            // 找到在外侧的点的索引
            for (int i = 0; i < bList.size(); ++i)
            {
                if (bList.at(i))
                {
                    indiceOut.push_back(i);
                }
                else
                {
                    inIdx = i;
                }
            }
            VertexOut &vertIn = vertice.at(inIdx);
            // 遍历外侧顶点，插值生成新的顶�?
            for (int i = 0; i < indiceOut.size(); ++i)
            {
                int index = indiceOut.at(i);
                VertexOut &vertOut = vertice.at(index);

                double factor = _getFactorForFarPlane(vertIn, vertOut);

                VertexOut vertNew = vertIn.interpolate(vertOut, factor);
                vertice[index] = vertNew;
            }
            tri.v1 = vertice[0];
            tri.v2 = vertice[1];
            tri.v3 = vertice[2];
            ++iter;
        }
        else if (outNum == 1)
        {
            // 如果有一个顶点在外部 ,构造一个梯形（修改旧三角形+添加一个新三角形）
            // 储存内侧顶点的索�?
            vector<int> indiceIn;
            // 外侧顶点的索�?
            int outIdx = 0;
            // 找到内侧的点的索�?
            for (int i = 0; i < bList.size(); ++i)
            {
                if (!bList.at(i))
                {
                    indiceIn.push_back(i);
                }
                else
                {
                    outIdx = i;
                }
            }
            VertexOut &vertOut = vertice.at(outIdx);
            // 遍历内侧顶点，插值生成新的顶点，并添加三角形
            vector<VertexOut> vertNewList;
            for (int i = 0; i < indiceIn.size(); ++i)
            {
                // 生成对应新顶�?
                int index = indiceIn.at(i);
                VertexOut vertIn = vertice.at(index);

                double factor = _getFactorForFarPlane(vertIn, vertOut);
                VertexOut vertNew = vertIn.interpolateEarly(vertOut, factor);
                vertNewList.push_back(vertIn);
                vertNewList.push_back(vertNew);
            }
            tri.v1 = vertNewList[0];
            tri.v2 = vertNewList[1];
            tri.v3 = vertNewList[2];

            Triangle triNew(vertNewList[1], vertNewList[2], vertNewList[3]);
            iter = triList.insert(iter + 1, triNew);
            ++iter;
        }
        else if (outNum == 3)
        {
            // 如果有三个顶点在外部，整体剔�?
            iter = triList.erase(iter);
        }
    }
}

void ClipUtil::_doClppingInCvvAgainstNearPlane(vector<Triangle> &triList)
{

    for (auto iter = triList.begin(); iter != triList.end(); /*do nothing*/)
    {
        Triangle &tri = *iter;
        
        vector<VertexOut> vertice = {tri.v1, tri.v2, tri.v3};
        vector<bool> bList = _getbOutListForNearPlane(tri);

        int outNum = 0;
        for (int i = 0; i < bList.size(); ++i)
        {
            bool out = bList.at(i);
            outNum = outNum + (out ? 1 : 0);
        }
        if (outNum == 0)
        {
            // 如果没有点在外部，不处理
            ++iter;
            continue;
        }
        else if (outNum == 2)
        {
            // 如果有两个顶点在外部，构造一个新三角形（修改旧三角形�?
            // 储存外侧顶点的索�?
            vector<int> indiceOut;
            // 内侧顶点的索�?
            int inIdx = 0;
            // 找到在外侧的点的索引
            for (int i = 0; i < bList.size(); ++i)
            {
                if (bList.at(i))
                {
                    indiceOut.push_back(i);
                }
                else
                {
                    inIdx = i;
                }
            }
            VertexOut &vertIn = vertice.at(inIdx);
            // 遍历外侧顶点，插值生成新的顶�?
            for (int i = 0; i < indiceOut.size(); ++i)
            {
                int index = indiceOut.at(i);
                VertexOut &vertOut = vertice.at(index);
                double factor = _getFactorForNearPlane(vertIn, vertOut);

                VertexOut vertNew = vertIn.interpolate(vertOut, factor);
                vertice[index] = vertNew;
            }
            tri.v1 = vertice[0];
            tri.v2 = vertice[1];
            tri.v3 = vertice[2];
            ++iter;
        }
        else if (outNum == 1)
        {
            // 如果有一个顶点在外部 ,构造一个梯形（修改旧三角形+添加一个新三角形）
            // 储存内侧顶点的索�?
            vector<int> indiceIn;
            // 外侧顶点的索�?
            int outIdx = 0;
            // 找到内侧的点的索�?
            for (int i = 0; i < bList.size(); ++i)
            {
                if (!bList.at(i))
                {
                    indiceIn.push_back(i);
                }
                else
                {
                    outIdx = i;
                }
            }
            VertexOut &vertOut = vertice.at(outIdx);
            // 遍历内侧顶点，插值生成新的顶点，并添加三角形
            vector<VertexOut> vertNewList;
            for (int i = 0; i < indiceIn.size(); ++i)
            {
                // 生成对应新顶�?
                int index = indiceIn.at(i);
                VertexOut vertIn = vertice.at(index);
                double factor = _getFactorForNearPlane(vertIn, vertOut);
                VertexOut vertNew = vertIn.interpolateEarly(vertOut, factor);
                vertNewList.push_back(vertIn);
                vertNewList.push_back(vertNew);
            }
            tri.v1 = vertNewList[0];
            tri.v2 = vertNewList[1];
            tri.v3 = vertNewList[2];

            Triangle triNew(vertNewList[1], vertNewList[2], vertNewList[3]);
            iter = triList.insert(iter + 1, triNew);
            ++iter;
        }
        else if (outNum == 3)
        {
            // 如果有三个顶点在外部，整体剔�?
            iter = triList.erase(iter);
        }
    }
}

vector<bool> ClipUtil::_getbOutListForNearPlane(Triangle &tri)
{
    Vec4 p1 = tri.v1.pos;
    Vec4 p2 = tri.v2.pos;
    Vec4 p3 = tri.v3.pos;

    bool bOut1 = p1.z < 0;
    bool bOut2 = p2.z < 0;
    bool bOut3 = p3.z < 0;

    vector<bool> bList = {bOut1, bOut2, bOut3};

    return bList;
}

vector<bool> ClipUtil::_getbOutListForFarPlane(Triangle &tri)
{
    Vec4 p1 = tri.v1.pos;
    Vec4 p2 = tri.v2.pos;
    Vec4 p3 = tri.v3.pos;
    
    bool bOut1 = p1.z > p1.w;
    bool bOut2 = p2.z > p2.w;
    bool bOut3 = p3.z > p3.w;

    vector<bool> bList = {bOut1, bOut2, bOut3};

    return bList;
}

vector<bool> ClipUtil::_getbOutListForRightPlane(Triangle &tri)
{
    Vec4 p1 = tri.v1.pos;
    Vec4 p2 = tri.v2.pos;
    Vec4 p3 = tri.v3.pos;

    bool bOut1 = p1.x > p1.w;
    bool bOut2 = p2.x > p2.w;
    bool bOut3 = p3.x > p3.w;

    vector<bool> bList = {bOut1, bOut2, bOut3};

    return bList;    
}

vector<bool> ClipUtil::_getbOutListForLeftPlane(Triangle &tri)
{
    Vec4 p1 = tri.v1.pos;
    Vec4 p2 = tri.v2.pos;
    Vec4 p3 = tri.v3.pos;

    bool bOut1 = p1.x < -p1.w;
    bool bOut2 = p2.x < -p2.w;
    bool bOut3 = p3.x < -p3.w;

    vector<bool> bList = {bOut1, bOut2, bOut3};

    return bList;
}

vector<bool> ClipUtil::_getbOutListForTopPlane(Triangle &tri)
{
    Vec4 p1 = tri.v1.pos;
    Vec4 p2 = tri.v2.pos;
    Vec4 p3 = tri.v3.pos;

    bool bOut1 = p1.y > p1.w;
    bool bOut2 = p2.y > p2.w;
    bool bOut3 = p3.y > p3.w;

    vector<bool> bList = {bOut1, bOut2, bOut3};

    return bList;
}

vector<bool> ClipUtil::_getbOutListForBottomPlane(Triangle &tri)
{
    Vec4 p1 = tri.v1.pos;
    Vec4 p2 = tri.v2.pos;
    Vec4 p3 = tri.v3.pos;

    bool bOut1 = p1.y < -p1.w;
    bool bOut2 = p2.y < -p2.w;
    bool bOut3 = p3.y < -p3.w;

    vector<bool> bList = {bOut1, bOut2, bOut3};

    return bList;
}
