#ifndef CSPXHPGLCOMMANDS_H
#define CSPXHPGLCOMMANDS_H

#include <QVector>
#include <QVector3D>

#include "../Containers/cspxlist.h"

class CSPXString;
class QByteArray;

class CSPXHpglCommands
{
    public:
        CSPXHpglCommands();

        struct s_command
        {
            char line[32];
            char code[32];
            int x;
            int y;
            int z;
            union
            {
                int tool_index;
                int wait_time;
            } u_tool_time;
            float xy_speed;
            float xyup_speed;
            float xydown_speed;
            float zdown_speed;
            float zup_speed;
        };

        int x_min;
        int x_max;
        int y_min;
        int y_max;
        int z_min;
        int z_max;

        int last_dx;
        int last_dy;
        int last_dz;

        int fileChunks;


    public: QVector<QVector3D> polyLine;

    public:CSPXList<struct s_command> commandList[10];

    public: int LoadCommands(const char *data, int length);
    public: bool TranslateXBy(int dx);
    public: bool TranslateYBy(int dy);
    public: bool Rotate();
    public: bool FlipX();
    public: bool FlipY();
    public: QByteArray* GetByteArray(int index);

};

#endif // CSPXHPGLCOMMANDS_H
