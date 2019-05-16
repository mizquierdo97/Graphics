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

    QComboBox* newComboBox = new QComboBox();
    textureComboBox.push_back(newComboBox);
    QComboBox* newComboBox2 = new QComboBox();
    textureComboBox.push_back(newComboBox2);
    ui->TexturesLayout->addWidget(newComboBox2);

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
        ui->TexturesLayout->update();
        for(int i = 0; i < mesh->submeshes.size(); i++)
        {
            QComboBox* newComboBox = new QComboBox();
            textureComboBox.push_back(newComboBox);
            ui->TexturesLayout->addWidget(newComboBox);
        }
    }
    static int a = 0;
    if(a != 0)
     resources->textureResources[0]->Load();
    a++;
}

