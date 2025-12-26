#ifndef CSPXTYPES_H
#define CSPXTYPES_H

#pragma once

struct s_rect
{
    int left;
    int right;
    int top;
    int bottom;
};

struct s_size
{
    int cx;
    int cy;
};

struct s_box
{
    int left;
    int right;
    int top;
    int bottom;
    int front;
    int back;
};

#endif // CSPXTYPES_H
