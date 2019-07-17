#include "opengldraw.h"

OpenGlDraw::OpenGlDraw(QString fn, QWidget *parent, bool toObj, bool showScene, int quality): QOpenGLWidget(parent), fileName(fn)
{
    plantvertex.ReadVertex(fileName.toStdString());
    this->quality = quality;

    resize(1200, 800);
    if(showScene) show();
    pObj = toObj;
    pShowScene = showScene;
    initializeVertex();
}

void OpenGlDraw::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DOUBLE);

    ourShader = new Shader("basic.vs", "basic.frag");
    camera = new Camera();
}

void OpenGlDraw::resizeGL(int w,int h)
{
    glViewport(0, 0, w, h);
    camera->updateScreenSize(w, h);
}

void OpenGlDraw::initializeVertex()
{
    std::ifstream stLeaf;
    stLeaf.open("leaf.vdat");
    while(stLeaf)
    {
        double x, y, z;
        stLeaf >> x >> y >> z;
        vertexLeaf.push_back(x);
        vertexLeaf.push_back(y);
        vertexLeaf.push_back(z);
    }
    vertexLeaf.erase(vertexLeaf.end() - 1);
    vertexLeaf.erase(vertexLeaf.end() - 1);
    vertexLeaf.erase(vertexLeaf.end() - 1);

    for(ullint i = vertexLeaf.size() / 2; i < vertexLeaf.size(); i+=9)
    {
        float t1,t2,t3;
        t1 = vertexLeaf[i + 3];
        t2 = vertexLeaf[i + 4];
        t3 = vertexLeaf[i + 5];
        vertexLeaf[i + 3] = vertexLeaf[i + 6];
        vertexLeaf[i + 4] = vertexLeaf[i + 7];
        vertexLeaf[i + 5] = vertexLeaf[i + 8];
        vertexLeaf[i + 6] = t1;
        vertexLeaf[i + 7] = t2;
        vertexLeaf[i + 8] = t3;
    }

    std::queue<Tree*> q1,q2;
    q1.push(plantvertex.startTree);
    while(!q1.empty())
    {
        while(!q1.empty())
        {
            Tree *tr = q1.front();
            q1.pop();
            for (ullint j = 0; j < tr->child.size(); j++)
            {
                if(tr->child[j]->type_tree == "t")
                    currentColor = vec3(0.50, 0.25, 0.01);
                else if(tr->child[j]->type_tree == "b")
                    currentColor = vec3(0.7, 0.3, 0.1);
                else if(tr->child[j]->type_tree == "s")
                    currentColor = vec3(0.7, 0.5, 0.1);
                else if(tr->child[j]->type_tree == "l")
                    currentColor = vec3(0.45, 0.68, 0.1);

                if ((tr->child[j]->type_tree != "l")&& (tr->child[j]->type_tree.size() == 1)) cilinder(tr, j);
                else if (tr->child[j]->type_tree == "l")
                {
                    cilinder(tr, j);
                    for(ullint l = 0; l < vertexLeaf.size(); l+=3)
                    {
                        float x = vertexLeaf[l] - vertexLeaf[0];
                        float y = vertexLeaf[l + 1] - vertexLeaf[1];
                        float z = vertexLeaf[l + 2] - vertexLeaf[2];
                        double cosQ = cos(tr->child[j]->parameters["angleQ"] * PI / 180), sinQ = sin(tr->child[j]->parameters["angleQ"] * PI / 180),
                                cosW = cos(tr->child[j]->parameters["angleW"] * PI / 180), sinW = sin(tr->child[j]->parameters["angleW"] * PI / 180);

                        float d = sqrt(x*x + y*y + z*z);

                        x += d * sinW * cosQ;
                        y += d * sinW * sinQ;
                        z += d * cosW;

                        x += tr->child[j]->vertex[0];
                        y += tr->child[j]->vertex[1];
                        z += tr->child[j]->vertex[2];

                        vertexPositionLeaf.push_back(x);
                        vertexPositionLeaf.push_back(y);
                        vertexPositionLeaf.push_back(z);

                        vertexColorLeaf.push_back(currentColor.x);
                        vertexColorLeaf.push_back(currentColor.y);
                        vertexColorLeaf.push_back(currentColor.z);

                        vertexNormalLeaf.push_back(0);
                        vertexNormalLeaf.push_back(1);
                        vertexNormalLeaf.push_back(0);
                    }
                }
                q2.push(tr->child[j]);
            }
        }
        q1 = q2;
        while (!q2.empty()) q2.pop();
    }

    vertexPositionLeafIndexStart = vertexPosition.size();
    for(ullint i = 0; i < vertexPositionLeaf.size(); i++)
    {
        vertexPosition.push_back(vertexPositionLeaf[i]);
        vertexColor.push_back(vertexColorLeaf[i]);
        vertexNormal.push_back(vertexNormalLeaf[i]);
    }

    indexTreeEnd = vertexPosition.size();
    if (pObj)
    {
        QString s;
        for(int i = fileName.size() - 5; i >= 0; i--)
        {
            if (fileName[i] == '/') break;
            s = fileName[i] + s;
        }
        s = "trees_obj//" + s + "obj";
        toObj(s.toStdString());
    }
    if (pShowScene) initializeBuffers();
}

void OpenGlDraw::initializeBuffers()
{
    glGenBuffers(1, &VBO["vertexPosition"]);
    glGenBuffers(1, &VBO["vertexColor"]);
    glGenBuffers(1, &VBO["vertexNormal"]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO["vertexPosition"]);
    glBufferData(GL_ARRAY_BUFFER, vertexPosition.size()*sizeof(float), reinterpret_cast<GLvoid*>(&vertexPosition[0]), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO["vertexColor"]);
    glBufferData(GL_ARRAY_BUFFER, vertexColor.size()*sizeof(float), reinterpret_cast<GLvoid*>(&vertexColor[0]), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO["vertexNormal"]);
    glBufferData(GL_ARRAY_BUFFER, vertexNormal.size()*sizeof(float), reinterpret_cast<GLvoid*>(&vertexNormal[0]), GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO["vertexPosition"]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, VBO["vertexColor"]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, VBO["vertexNormal"]);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

void OpenGlDraw::toObj(string file_path)
{
    vector<ullint> vertexIndices, normalIndices;
    vector<vec3> temp_vertices, temp_normals;
    vec3 temp;

    for(ullint i = 0; i < vertexPosition.size(); i+=3)
    {
        temp = vec3(vertexPosition[i], vertexPosition[i+1], vertexPosition[i+2]);
        bool new_vertex = true;
        for(int j = temp_vertices.size() - 1; j >= 0; j--)
        {
            if ((fabs(temp_vertices[j].x - temp.x) < 0.01) && (fabs(temp_vertices[j].y - temp.y) < 0.01) && (fabs(temp_vertices[j].z - temp.z) < 0.01))
            {
                vertexIndices.push_back(j + 1);
                normalIndices.push_back(j + 1);
                new_vertex = false;
                break;
            }
        }
        if (!new_vertex) continue;

        temp_vertices.push_back(temp);

        temp = vec3(vertexNormal[i], vertexNormal[i+1], vertexNormal[i+2]);
        temp_normals.push_back(temp);

        vertexIndices.push_back(temp_normals.size());
        normalIndices.push_back(temp_normals.size());
    }
    fstream fs;
    fs.open(file_path, ios::out);

    for(ullint i = 0; i < temp_vertices.size(); i++)
        fs << "v " << temp_vertices[i].x << " " << temp_vertices[i].y << " " << temp_vertices[i].z << endl;
    for(ullint i = 0; i < temp_vertices.size(); i++)
        fs << "vn " << temp_normals[i].x << " " << temp_normals[i].y << " " << temp_normals[i].z << endl;
    for(ullint i = 0; i < vertexIndices.size(); i+=3)
    {
        fs << "f " << vertexIndices[i] << "//" << normalIndices[i];
        fs << " "<< vertexIndices[i+1] << "//" << normalIndices[i+1];
        fs << " " << vertexIndices[i+2] << "//" << normalIndices[i+2] << endl;
    }

    fs.close();
}

void OpenGlDraw::paintGL()
{
    glEnable(GL_DEPTH_TEST);
    static const GLfloat clearColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, clearColor);

    ourShader->Use();
    glBindVertexArray(VAO);

    GLint LightPositionLoc = glGetUniformLocation(ourShader->programHandle, "LightPosition");
    glm::vec4 LightPosition = glm::vec4(0.0, 0.0, 1000.0, 0);
    glUniform4fv(LightPositionLoc, 1, glm::value_ptr(LightPosition));

    GLint cameraMatrix = glGetUniformLocation(ourShader->programHandle, "camera");
    glUniformMatrix4fv(cameraMatrix, 1, GL_FALSE, glm::value_ptr(camera->GetCamera()));

    GLint viewMatrix = glGetUniformLocation(ourShader->programHandle, "view");
    glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(camera->GetView()));

    GLint modelMatrix = glGetUniformLocation(ourShader->programHandle, "model");
    glm::mat4 modelTransform = glm::mat4(1.0);
    glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(modelTransform));

    GLint normalMatrix = glGetUniformLocation(ourShader->programHandle, "normal");
    glm::mat4  mv = camera->GetView() * modelTransform;
    glm::mat3 normal_m = glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2]));
    glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(normal_m));

    glDrawArrays(GL_TRIANGLES, 0, indexTreeEnd / 3);

    glPointSize(3);
    glDrawArrays(GL_POINTS, indexTreeEnd / 3, (vertexPosition.size() - indexTreeEnd) / 3);
}

void OpenGlDraw::cilinder(Tree *&tr, int j)
{
    vec3 tempVertex1, tempVertex2;
    bool firstVertex = true;

    for(int i = 0; i < quality+1; i++)
    {
      double seta = i*360.0/quality;
      double setaRad = seta * PI / 180;
      double d = sqrt(pow(tr->vertex[0] - tr->child[j]->vertex[0], 2) + pow(tr->vertex[1] - tr->child[j]->vertex[1], 2) +pow(tr->vertex[2] - tr->child[j]->vertex[2], 2));

      double x = sin(setaRad) * tr->trunk_thickness, y = 0, z = cos(setaRad) * tr->trunk_thickness;
      double x1 = sin(setaRad) * tr->child[j]->trunk_thickness, y1 = 0, z1 = cos(setaRad) * tr->child[j]->trunk_thickness;

      double w,q = 90;
      vector<double> tempcoord = {-tr->child[j]->vertex[0] + tr->vertex[0],
                                 -tr->child[j]->vertex[1] + tr->vertex[1],
                                 -tr->child[j]->vertex[2] + tr->vertex[2]};
      if ((tempcoord[0] > 0) && (tempcoord[1] >= 0)) q = atan(tempcoord[1] / tempcoord[0]) * 180 / PI;
      else if (tempcoord[0] < 0)  q = 180 + atan(tempcoord[1] / tempcoord[0]) * 180 / PI;
      else if ((tempcoord[0] > 0) && (tempcoord[1] < 0)) q = 360 + atan(tempcoord[1] / tempcoord[0]) * 180 / PI;
      else if ((tempcoord[0] == 0) && (tempcoord[1] == 0)) q = 90;

      if(pow(tempcoord[0], 2) + pow(tempcoord[1], 2) + pow(tempcoord[2], 2) == 0)
          w = acos(0) * 180 / PI;
      else
        w = acos(tempcoord[2] / sqrt(pow(tempcoord[0], 2) + pow(tempcoord[1], 2) + pow(tempcoord[2], 2))) * 180 / PI;

      q +=90;
      w -= 90;

      q = q * PI / 180;
      w = w * PI / 180;
      double t;

      t = x;
      x = t * cos(q) - y * sin(q);
      y = t * sin(q) + y * cos(q);
      t = x1;
      x1 = t * cos(q) - y1 * sin(q);
      y1 = t * sin(q) + y1 * cos(q);

      t = x;
      x = t * cos(w) + z * sin(w);
      z = -t * sin(w) + z * cos(w);
      t = x1;
      x1 = t * cos(w) + z1 * sin(w);
      z1 = -t * sin(w) + z1 * cos(w);

      x += tr->vertex[0];
      y += tr->vertex[1];
      z += tr->vertex[2];
      x1 += tr->child[j]->vertex[0];
      y1 += tr->child[j]->vertex[1];
      z1 += tr->child[j]->vertex[2];

      if (!firstVertex)
      {
          vertexPosition.push_back(tempVertex1.x);
          vertexPosition.push_back(tempVertex1.y);
          vertexPosition.push_back(tempVertex1.z);

          vertexPosition.push_back(x);
          vertexPosition.push_back(y);
          vertexPosition.push_back(z);

          vertexPosition.push_back(tempVertex2.x);
          vertexPosition.push_back(tempVertex2.y);
          vertexPosition.push_back(tempVertex2.z);

          vertexPosition.push_back(tempVertex2.x);
          vertexPosition.push_back(tempVertex2.y);
          vertexPosition.push_back(tempVertex2.z);

          vertexPosition.push_back(x);
          vertexPosition.push_back(y);
          vertexPosition.push_back(z);

          vertexPosition.push_back(x1);
          vertexPosition.push_back(y1);
          vertexPosition.push_back(z1);

          for(int i = 0; i < 6; i++)
          {
              vertexColor.push_back(currentColor.x);
              vertexColor.push_back(currentColor.y);
              vertexColor.push_back(currentColor.z);
          }

          vec3 a = vec3(x - tempVertex1.x, y - tempVertex1.y, z - tempVertex1.z);
          vec3 b = vec3(tempVertex2.x - tempVertex1.x, tempVertex2.y - tempVertex1.y, tempVertex2.z - tempVertex1.z);
          vec3 norm = glm::cross(a, b);

          for(int i = 0; i < 3; i++)
          {
              vertexNormal.push_back(norm.x);
              vertexNormal.push_back(norm.y);
              vertexNormal.push_back(norm.z);
          }

          a = vec3(x1 - tempVertex2.x, y1 - tempVertex2.y, z1 - tempVertex2.z);
          b = vec3(x - tempVertex2.x, y - tempVertex2.y, z - tempVertex2.z);
          norm = glm::cross(b, a);

          for(int i = 0; i < 3; i++)
          {
              vertexNormal.push_back(norm.x);
              vertexNormal.push_back(norm.y);
              vertexNormal.push_back(norm.z);
          }

      }
      else firstVertex = false;

      tempVertex1 = vec3(x, y, z);
      tempVertex2 = vec3(x1, y1, z1);

    }
}

void OpenGlDraw::keyPressEvent(QKeyEvent *event)
{
     if(event->key() == 87) //w
     {
         camera->ProcessKeyboard(FORWARD);
         update();
     }
     if(event->key() == 83) //s
     {
         camera->ProcessKeyboard(BACKWARD);
         update();
     }
     if(event->key() == 65) //a
     {
         camera->ProcessKeyboard(LEFT);
         update();
     }
     if(event->key() == 68) //d
     {
         camera->ProcessKeyboard(RIGHT);
         update();
     }
}

void OpenGlDraw::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::RightButton)
    {
        bufMR.x = event->x();
        bufMR.y = event->y();
    }
}

void OpenGlDraw::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::RightButton)
    {
        vec2 buf;
        buf.x = event->x();
        buf.y = event->y();

        float xoffset = buf.x - bufMR.x;
        float yoffset = buf.y - bufMR.y;


        xoffset /= 20;
        yoffset /= 20;

        camera->ProcessMouseMovement(xoffset, yoffset);

        bufMR = buf;
        update();
    }
}

void OpenGlDraw::wheelEvent(QWheelEvent *event)
{
    if (event->delta())
    {
        float del = static_cast<float>(event->delta() / 2000);
        camera->ProcessMouseScroll(del);
        update();
    }
}
