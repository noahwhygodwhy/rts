
bool intersecting(vec2 minA, vec2 maxA, vec2 minB, vec2 maxB)
{
    if (minA.x > maxB.x || minB.x > maxA.x)
    {
        return false;
    }
    if (minA.y > maxB.y || minB.y > maxA.y)
    {
        return false;
    }
    return true;
}