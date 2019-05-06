#include "resources.h"
#include <QDir>

Resources::Resources()
{

}

void Resources::LoadResources(QString path)
{
    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    QFileInfoList list = dir.entryInfoList();

    for(int i = 0; i < list.size(); i++)
    {
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.isDir())
        {
            LoadResources(fileInfo.filePath());
        }
        else
        {
            QFileInfo file;
            file.setFile(fileInfo.filePath());
            QString type = file.completeSuffix();

            if(type.compare("png") == 0|| type.compare("jpg") == 0|| type.compare("tga") == 0)
            {

            }
            else if(type.compare("obj") == 0|| type.compare("fbx")== 0)
            {
                int a = 0;
                a = 1;
            }
        }

    }

}
