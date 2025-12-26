#include "cspxhpglcommands.h"
#include <stdio.h>
#include <math.h>

#include <QByteArray>

CSPXHpglCommands::CSPXHpglCommands()
{
    x_min = 10000;
    x_max = -10000;
    y_min = 10000;
    y_max = -10000;
    z_min = 10000;
    z_max = -10000;

    fileChunks = 0;
}

/*
    @CA     Contouring angle set
    @CL     Contouring Length of segment
    @CP     Contouring Parameters output
    @DB     Digital Bit Output
    @DI     Digital Input Reading
    @DO     Digital Output Setting
    @EX     Exit to DCC mode
    @HM     Home all 3 axes
    @HZ     Home Z-axis only
    @NH     Set Current Position as New XYZ-Home
    @NO     Set Current Position as New XY-Origin
    @NZ     Set Current Position as New Z-Home
    @OC     Display & Output absolute position
    @SM     Set spindle Motor
    @SS     Set Spindle Speed
    @TC     Tool Change output
    @WT     Wait x msec
    @XX     Reset System
    @ZA     Z Move Absolute
    @ZD     Z Down velocity
    @ZP     Z Up/Down Positions setting
    @ZR     Z Move Relative
    @ZS     Z Up/Down Moving Speed
    @ZU     Z Up velocity
    @ZV     Z Up/Down Travelling Speed
    @ZW     Z Up/Down Waiting Time

    DF Defaults Restore
    OA Output Actual Position
    OC Output Commanded Position
    OE Output Error
    OF Output Scale Factors
    OI Output Identification
    PA Plot Absolute
    PD Pen Down
    PR Plot Relative
    PU Pen Up
    SP Select Pen
    VS Velocity Select

 */

/* Splits the file into tool associated chunks*/
int CSPXHpglCommands::LoadCommands(const char *data, int length)
{
    const char* pdata = data;
    char command[32];
    int limit = 32;
    char spindle_on_off;
    int z_coord;
    int x_coord;
    int y_coord;
    float z_up_speed;
    float z_down_speed;
    float z_travel_speed;
    float xy_travel_speed;
    float xy_mill_speed;
    int wait_time;
    int tool_index;

    unsigned int fileIndex = 0;

    struct s_command scommand;

    while(*pdata != '\0')
    {
        // remove <cr> and <nl>
        while(*pdata == '\r' || *pdata == '\n' || *pdata == ' ')
            pdata++;

        memset(command,0,32);
        char *pCommand = &command[0];
        int commandLength = 0;
        while(*pdata != '\0' && *pdata != ';' && commandLength < limit)
        {
            if (*pdata != ' ')
            {
                *pCommand=*pdata;
                pCommand++;
                commandLength++;
            }
            pdata++;
        }

        pdata++; //next command or cr,nl

        memset(&scommand,0,sizeof(struct s_command));
        if (command[0] == '@')
        {
            if (command[1] == 'Z' && command[2] == 'A') //@ZA
            {
                sscanf(&command[3],"%d",&z_coord);
                strcpy(scommand.code,"@ZA");
                scommand.z = z_coord;
                z_max = fmax(scommand.z,z_max);
                z_min = fmin(scommand.z,z_min);
            }
            else if (command[1] == 'Z' && command[2] == 'R') //@ZR
            {
                sscanf(&command[3],"%d",&z_coord);
                strcpy(scommand.code,"@ZR");
                scommand.z = z_coord;
            }
            else if (command[1] == 'Z' && command[2] == 'U') //@ZU
            {
                sscanf(&command[3],"%f",&z_up_speed);
                strcpy(scommand.code,"@ZU");
                scommand.zup_speed = z_up_speed;
            }
            else if (command[1] == 'Z' && command[2] == 'D') //@ZD
            {
                sscanf(&command[3],"%f",&z_down_speed);
                strcpy(scommand.code,"@ZD");
                scommand.zdown_speed = z_down_speed;
            }
            else if (command[1] == 'Z' && command[2] == 'S') //@ZS
            {
                sscanf(&command[3],"%f,%f",&z_up_speed,&z_down_speed);

                strcpy(scommand.code,"@ZS");
                scommand.zup_speed = z_up_speed;
                scommand.zdown_speed = z_down_speed;
            }
            else if (command[1] == 'Z' && command[2] == 'V') //@ZV
            {
                sscanf(&command[3],"%f,%f",&xy_travel_speed,&xy_mill_speed);
                strcpy(scommand.code,"@ZV");
                scommand.xyup_speed = xy_travel_speed;
                scommand.xydown_speed = xy_mill_speed;
            }
            else if (command[1] == 'Z' && command[2] == 'W') //@ZW
            {
                sscanf(&command[3],"%d",&wait_time);
                strcpy(scommand.code,"@ZW");
                scommand.u_tool_time.wait_time = wait_time;
            }
            else if (command[1] == 'Z' && command[2] == 'P') //@ZP
            {

            }
            else if (command[1] == 'S' && command[2] == 'S') //@SS
            {
                sscanf(&command[3],"%c",&spindle_on_off);
                scommand.u_tool_time.wait_time = spindle_on_off;
            }
            else if (command[1] == 'W' && command[2] == 'T') //@WT
            {
                sscanf(&command[3],"%d",&wait_time);
                strcpy(scommand.code,"@WT");
                scommand.u_tool_time.wait_time = wait_time;
            }
            else if (command[1] == 'T' && command[2] == 'C') //@TC
            {

            }
        }
        else
        {
            if (command[0] == 'T') //Tool change
            {
                fileIndex++;
                sscanf(&command[0],"T:%d",&tool_index);
                strcpy(scommand.code,&command[0]);
                scommand.u_tool_time.tool_index = tool_index;
           }
            else if (command[0] == 'V' && command[1] == 'S') //VS
            {
                sscanf(&command[2],"%f",&xy_travel_speed);
                strcpy(scommand.code,"VS");
                scommand.xy_speed = xy_travel_speed;
            }
            else if (command[0] == 'P' && command[1] == 'A') //PA
            {
                sscanf(&command[2],"%d,%d",&x_coord,&y_coord);
                strcpy(scommand.code,"PA");
                scommand.x = x_coord;
                scommand.y = y_coord;
                x_max = fmax(scommand.x,x_max);
                y_max = fmax(scommand.y,y_max);
                x_min = fmin(scommand.x,x_min);
                y_min = fmin(scommand.y,y_min);
            }
            else if (command[0] == 'P' && command[1] == 'R') //PR
            {
                sscanf(&command[2],"%d,%d",&x_coord,&y_coord);
                strcpy(scommand.code,"PR");
                scommand.x = x_coord;
                scommand.y = y_coord;
            }
            else if (command[0] == 'P' && command[1] == 'U') //PU
            {
                strcpy(scommand.code,"PU");
            }
            else if (command[0] == 'P' && command[1] == 'D') //PD
            {
                strcpy(scommand.code,"PD");
            }
            else if (command[0] == 'O' && command[1] == 'A') //OA
            {
                strcpy(scommand.code,"OA");
            }
            else if (command[0] == 'O' && command[1] == 'C') //OC
            {
                strcpy(scommand.code,"OC");
            }
            else if (command[0] == 'O' && command[1] == 'F') //OF
            {
                strcpy(scommand.code,"OF");
            }
            else if (command[0] == 'O' && command[1] == 'I') //OI
            {
                strcpy(scommand.code,"OI");
            }

        }
        if (scommand.code[0] != '\0')
        {
            struct s_command *pscommand = new struct s_command;
            memcpy(pscommand,&scommand,sizeof (struct s_command));

            commandList[fileIndex].Append(pscommand,true);

            fileChunks = fileIndex +1;
        }
    }

    return 0;
}

bool CSPXHpglCommands::TranslateBy(int dx, int dy, int dz)
{

    for (int i = 0; i < fileChunks; i++)
    {
        CSPXListNode<struct s_command> *cmdNodes = commandList[i].GetHead();
        while(cmdNodes)
        {
            struct s_command *cmd = cmdNodes->GetElement();
            if (!strcmp(cmd->code,"PA"))
            {
                cmd->x += dx;
                cmd->y += dy;
            }
            else if (!strcmp(cmd->code,"@ZA"))
            {
                cmd->z += dz;
            }
            cmdNodes = cmdNodes->GetNext();
        }

    }

    x_min += dx;
    x_max += dx;
    y_min += dy;
    y_max += dy;
    z_min += dz;
    z_max += dz;

    return true;
}

bool CSPXHpglCommands::FlipY()
{
    int cy = (y_max-y_min)/2.;

    for (int i = 0; i < fileChunks; i++)
    {
        CSPXListNode<struct s_command> *cmdNodes = commandList[i].GetHead();
        while(cmdNodes)
        {
            struct s_command *cmd = cmdNodes->GetElement();
            if (!strcmp(cmd->code,"PA") || !strcmp(cmd->code,"PR"))
            {
                cmd->y += (cy - cmd->y);
                y_min = fmin(cmd->y,y_min);
                y_max = fmin(cmd->y,y_max);
            }
            cmdNodes = cmdNodes->GetNext();
        }

    }

    return true;
}

bool CSPXHpglCommands::FlipX()
{

    int cx = (x_max-x_min)/2.;

    for (int i = 0; i < fileChunks; i++)
    {
        CSPXListNode<struct s_command> *cmdNodes = commandList[i].GetHead();
        while(cmdNodes)
        {
            struct s_command *cmd = cmdNodes->GetElement();
            if (!strcmp(cmd->code,"PA") || !strcmp(cmd->code,"PR"))
            {
                cmd->x += (cx - cmd->x);

                x_min = fmin(cmd->x,x_min);
                x_max = fmin(cmd->x,x_max);
            }
            cmdNodes = cmdNodes->GetNext();
        }

    }

    return true;
}


bool CSPXHpglCommands::Rotate()
{
    x_min = 10000;
    x_max = -10000;
    y_min = 10000;
    y_max = -10000;

    int temp;
    for (int i = 0; i < fileChunks; i++)
    {
        CSPXListNode<struct s_command> *cmdNodes = commandList[i].GetHead();
        while(cmdNodes)
        {
            struct s_command *cmd = cmdNodes->GetElement();
            if (!strcmp(cmd->code,"PA") || !strcmp(cmd->code,"PR"))
            {
                temp = cmd->x;
                cmd->x = cmd->y;
                cmd->y = temp;
                x_min = fmin(cmd->x,x_min);
                y_min = fmin(cmd->y,y_min);
                x_max = fmin(cmd->x,x_max);
                y_max = fmin(cmd->y,y_max);
            }
            cmdNodes = cmdNodes->GetNext();
        }

    }

    return true;
}


QByteArray* CSPXHpglCommands::GetByteArray(int index)
{
    char command[64];

    QByteArray *data = new QByteArray;

    CSPXListNode<struct s_command> *cmdNodes = commandList[index].GetHead();
    while(cmdNodes)
    {
        struct s_command *cmd = cmdNodes->GetElement();

        memset(&command[0],0,64);
        if (!strcmp(cmd->code,"@ZA"))
        {
            sprintf(&command[0],"@ZA %d;",cmd->z);
            data->append(command);
        }
        else if (!strcmp(cmd->code,"@ZR"))
        {
            sprintf(&command[0],"@ZR %d;",cmd->z);
            data->append(command);
        }
        else if (!strcmp(cmd->code,"@ZU"))
        {
            sprintf(&command[0],"@ZU %f;",cmd->zup_speed);
            data->append(command);
        }
        else if (!strcmp(cmd->code,"@ZD"))
        {
            sprintf(&command[0],"@ZD %f;",cmd->zdown_speed);
            data->append(command);
        }
        else if (!strcmp(cmd->code,"@ZS"))
        {
            sprintf(&command[0],"@ZS %f,%f;",cmd->zup_speed,cmd->zdown_speed);
            data->append(command);
        }
        else if (!strcmp(cmd->code,"@ZV")) //@ZV
        {
            sprintf(&command[0],"@ZV %f%f;",cmd->xyup_speed,cmd->xydown_speed);
            data->append(command);
        }
        else if (!strcmp(cmd->code,"@ZW")) //@ZW
        {
            sprintf(&command[0],"@ZW %d;",cmd->u_tool_time.wait_time);
            data->append(command);
        }
        else if (!strcmp(cmd->code,"@ZP")) //@ZP
        {

        }
        else if (!strcmp(cmd->code,"@SS")) //@SS
        {
            sprintf(&command[0],"@ZW %d;",cmd->u_tool_time.wait_time);
            data->append(command);
        }
        else if (!strcmp(cmd->code,"@WT")) //@WT
        {
            sprintf(&command[0],"@ZW %d;",cmd->u_tool_time.wait_time);
            data->append(command);
        }
        else if (!strcmp(cmd->code,"@TC")) //@TC
        {

        }
        if (!strcmp(cmd->code,"T")) //Tool change
        {
            sprintf(&command[0],"%s;",cmd->code);
            data->append(command);
        }
        else if (!strcmp(cmd->code,"VS")) //VS
        {
            sprintf(&command[0],"VS %f;",cmd->xy_speed);
            data->append(command);
        }
        else if (!strcmp(cmd->code,"PA")) //PA
        {
            sprintf(&command[0],"PA %d,%d;",cmd->x,cmd->y);
            data->append(command);
        }
        else if (!strcmp(cmd->code,"PR")) //PR
        {
            sprintf(&command[0],"PR %d,%d;",cmd->x,cmd->y);
            data->append(command);
        }
        else if (!strcmp(cmd->code,"PU")) //PU
        {
            sprintf(&command[0],"PU;");
            data->append(command);
        }
        else if (!strcmp(cmd->code,"PD")) //PD
        {
            sprintf(&command[0],"PD;");
            data->append(command);
        }
        else if (!strcmp(cmd->code,"OA")) //OA
        {
            data->append("OA;");
        }
        else if (!strcmp(cmd->code,"OC")) //OC
        {
            data->append("OC;");
        }
        else if (!strcmp(cmd->code,"OF")) //OF
        {
            data->append("OF;");
        }
        else if (!strcmp(cmd->code,"OI")) //OI
        {
            data->append("OI;");
        }

        cmdNodes = cmdNodes->GetNext();
    }

    return data;
}
