#pragma once

class ConstType
{
public:
    static const int INT = 0;
    static const int CHAR = 1;
    static const int FLOAT = 2;
    static const int BOOLEAN = 3;
    static const int STRING = 4;
    static const int NOT_CONSTANT = -1;

    ConstType();
};