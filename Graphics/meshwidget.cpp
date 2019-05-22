#include "meshwidget.h"
#include "ui_meshwidget.h"
#include "componentmesh.h"
#include "hierarchy.h"
#include "resources.h"
#include "resourcetexture.h"
MeshWidget::MeshWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeshWidget)
{
    ui->setupUi(this);


}

MeshWidget::~MeshWidget()
{
    delete ui;
}

void MeshWidget::Update(Object *selected)
{
        ComponentMesh* compMesh =  selected->GetComponentMesh();
        int selectedIndex = compMesh->index;
        ui->comboBox->setCurrentIndex(selectedIndex);
}

void MeshWidget::on_comboBox_currentIndexChanged(int _index)
{

    if(hierarchy->selectedObject != nullptr)
    {
        ComponentMesh* selectedMesh = hierarchy->selectedObject->GetComponentMesh();
        selectedMesh->index = _index;
        selectedMesh->resourceMesh = resources->meshResources[_index];

        if(selectedMesh->resourceMesh->numUses == 0)
        {
            selectedMesh->resourceMesh->numUses++;
            if( selectedMesh->resourceMesh->mesh == nullptr)
                 selectedMesh->resourceMesh->mesh = new Mesh();
            QString path = selectedMesh->resourceMesh->path;
            selectedMesh->resourceMesh->mesh->loadModel(path);
        }
        if(selectedMesh->resourceMesh == nullptr || selectedMesh->resourceMesh->mesh == nullptr)
            return;
        Mesh* mesh = selectedMesh->resourceMesh->mesh;

        for(int i = 0; i < textureComboBox.size(); i++)
        {
            ui->TexturesLayout->removeWidget(textureComboBox[i]);
            delete textureComboBox[i];
        }
        textureComboBox.clear();
        ui->scrollArea->setLayout(ui->TexturesLayout);
        ui->TexturesLayout->update();
        for(int i = 0; i < mesh->submeshes.size(); i++)
        {
            QComboBox* newComboBox = new QComboBox();
            textureComboBox.push_back(newComboBox);
            ui->TexturesLayout->addWidget(newComboBox);
            QVector<ResourceTexture*> texRes = resources->textureResources;
            for(int n = 0; n < texRes.size(); n++)
            {
                newComboBox->addItem(texRes[n]->name);

            }
            QString name = "ComboBox";
            name += QString::number(i);
            newComboBox->setObjectName(name);


            connect(newComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onTextureComboChanged(const QString&)));
        }
    }

}

void MeshWidget::onTextureComboChanged(const QString& stringText)
{
    QString name = sender()->objectName();
    for(int i = 0; i < textureComboBox.size(); i++)
    {
        if(textureComboBox[i]->objectName().compare(name) == 0)
        {
            for(int n = 0; n < resources->textureResources.size(); n++)
            {
                if(resources->textureResources[n]->name.compare(stringText) == 0)
                {
                    resources->textureResources[n]->Load();
                     ComponentMesh* selectedMesh = hierarchy->selectedObject->GetComponentMesh();
                     selectedMesh->resourceMesh->mesh->submeshes[i]->GLTexture = resources->textureResources[n]->GLTexture;
                }
            }
        }
    }
}
