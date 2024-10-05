// Βενιόπουλος Δημήτρης 3610
// Αθανάσιος Γιαπουτζής 3589

#include <GL/glut.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <math.h>

float camera_theta = 0;
int animation_type = 0;
int play_animation = 0;
int block_animation = 0;
float animation_speed = 3;
float theta[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float target_degrees[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float max_degrees_in_animation = 0;

GLfloat tetrahedron[4][3] = {
        {0, 0, 1},
        {0, 0.942809, -0.33333},
        {-0.816497, -0.471405, -0.333333},
        {0.816497, -0.471405, -0.333333}
};

typedef struct node{
    GLfloat m[16];
    void (*f)();
    struct node *sibling;
    struct node *child;
}node;

node torso_node, neck_node, head_node, lufl_node, rufl_node, lubl_node, rubl_node, llfl_node, rlfl_node, llbl_node, rlbl_node, lfp_node, rfp_node, lbp_node, rbp_node;

void initialize()
{
    glClearColor(1, 1, 1, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-600, 600,-100 , 700,-1500,1500);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void normalize(float* v)
{
    float d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] /= d;
    v[1] /= d;
    v[2] /= d;
}

void recursive_subdivision(float a[], float b[], float c[], int step)
{
    if (step > 0) {
        float ab[3], ac[3], bc[3];
        for (int i = 0; i < 3; i++){
            ab[i] = a[i] + b[i];
            ac[i] = a[i] + c[i];
            bc[i] = b[i] + c[i];
        }
        normalize(ab);
        normalize(ac);
        normalize(bc);

        recursive_subdivision(a, ab, ac, step - 1);
        recursive_subdivision(b, bc, ab, step - 1);
        recursive_subdivision(c, ac, bc, step - 1);
        recursive_subdivision(ab, bc, ac, step - 1);
    }
    else {
        glBegin(GL_POLYGON);
        glVertex3fv(a);
        glVertex3fv(b);
        glVertex3fv(c);
        glEnd();
    }
}

void drawCircle(float r, float x, float y) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 360; i++)
        glVertex2f(r*cos(M_PI * i / 180.0) + x, r*sin(M_PI * i / 180.0) + y);
    glEnd();
}

void traverse(node* root){
    if (root == NULL)
        return;
    glPushMatrix();
    glMultMatrixf(root->m);
    root->f();
    if(root->child != NULL)
        traverse(root->child);
    glPopMatrix();
    if(root->sibling != NULL)
        traverse(root->sibling);
}

void torso()
{
    glColor3ub(144,84,47);
    glPushMatrix();
    glPushMatrix();
    glTranslatef(-200,280,-50);
    glRotatef(90,0,1,0);
    gluCylinder(gluNewQuadric(),70,70,440,100,100);
    drawCircle(70,0,0);
    glTranslatef(0,0,440);
    drawCircle(70,0,0);
    glPopMatrix();
}

void neck()
{
    glColor3ub(164,116,73);
    glPushMatrix();
    glTranslatef(220,300,-50);
    glRotatef(30,0,0,1);
    glRotatef(90,0,1,0);
    gluCylinder(gluNewQuadric(),40,40,100,100,100);
    drawCircle(40,0,0);
    glPopMatrix();
}

void head()
{
    glColor3ub(144,84,47);
    glPushMatrix();
    glTranslatef(330,360,-50);
    glScalef(60,60,60);
    recursive_subdivision(tetrahedron[0], tetrahedron[2], tetrahedron[1], 4);
    recursive_subdivision(tetrahedron[0], tetrahedron[3], tetrahedron[2], 4);
    recursive_subdivision(tetrahedron[0], tetrahedron[1], tetrahedron[3], 4);
    recursive_subdivision(tetrahedron[1], tetrahedron[2], tetrahedron[3], 4);
    glPopMatrix();
}

void leftUpperFrontLeg()
{
    glColor3ub(164,116,73);
    glPushMatrix();
    glTranslatef(220,270,-100);
    glRotatef(90,1,0,0);
    gluCylinder(gluNewQuadric(),20,20,120,100,100);
    drawCircle(20,0,0);
    glTranslatef(0,0,120);
    drawCircle(20,0,0);
    glPopMatrix();
}

void leftLowerFrontLeg()
{
    glColor3ub(144,84,47);
    glPushMatrix();
    glTranslatef(220,180,-100);
    glRotatef(90,1,0,0);
    gluCylinder(gluNewQuadric(),20,20,150,100,100);
    drawCircle(20,0,0);
    glTranslatef(0,0,150);
    drawCircle(20,0,0);
    glPopMatrix();
}

void leftFrontPaw()
{
    glColor3ub(81,68,61);
    glPushMatrix();
    glTranslatef(200,50,-100);
    glRotatef(90,0,1,0);
    gluCylinder(gluNewQuadric(),20,20,100,100,100);
    drawCircle(20,0,0);
    glTranslatef(0,0,100);
    drawCircle(20,0,0);
    glPopMatrix();
}

void rightUpperFrontLeg()
{
    glColor3ub(164,116,73);
    glPushMatrix();
    glTranslatef(220,270,0);
    glRotatef(90,1,0,0);
    gluCylinder(gluNewQuadric(),20,20,120,100,100);
    drawCircle(20,0,0);
    glTranslatef(0,0,120);
    drawCircle(20,0,0);
    glPopMatrix();
}

void rightLowerFrontLeg()
{
    glColor3ub(144,84,47);
    glPushMatrix();
    glTranslatef(220,180,0);
    glRotatef(90,1,0,0);
    gluCylinder(gluNewQuadric(),20,20,150,100,100);
    drawCircle(20,0,0);
    glTranslatef(0,0,150);
    drawCircle(20,0,0);
    glPopMatrix();
}

void rightFrontPaw()
{
    glColor3ub(81,68,61);
    glPushMatrix();
    glTranslatef(200,50,0);
    glRotatef(90,0,1,0);
    gluCylinder(gluNewQuadric(),20,20,100,100,100);
    drawCircle(20,0,0);
    glTranslatef(0,0,100);
    drawCircle(20,0,0);
    glPopMatrix();
}

void leftUpperBackLeg()
{
    glColor3ub(164,116,73);
    glPushMatrix();
    glTranslatef(-180,270,-100);
    glRotatef(90,1,0,0);
    gluCylinder(gluNewQuadric(),20,20,120,100,100);
    drawCircle(20,0,0);
    glTranslatef(0,0,120);
    drawCircle(20,0,0);
    glPopMatrix();
}

void leftLowerBackLeg()
{
    glColor3ub(144,84,47);
    glPushMatrix();
    glTranslatef(-180,180,-100);
    glRotatef(90,1,0,0);
    gluCylinder(gluNewQuadric(),20,20,150,100,100);
    drawCircle(20,0,0);
    glTranslatef(0,0,150);
    drawCircle(20,0,0);
    glPopMatrix();
}

void leftBackPaw()
{
    glColor3ub(81,68,61);
    glPushMatrix();
    glTranslatef(-200,50,-100);
    glRotatef(90,0,1,0);
    gluCylinder(gluNewQuadric(),20,20,100,100,100);
    drawCircle(20,0,0);
    glTranslatef(0,0,100);
    drawCircle(20,0,0);
    glPopMatrix();
}

void rightUpperBackLeg()
{
    glColor3ub(164,116,73);
    glPushMatrix();
    glTranslatef(-180,270,0);
    glRotatef(90,1,0,0);
    gluCylinder(gluNewQuadric(),20,20,120,100,100);
    drawCircle(20,0,0);
    glTranslatef(0,0,120);
    drawCircle(20,0,0);
    glPopMatrix();
}

void rightLowerBackLeg()
{
    glColor3ub(144,84,47);
    glPushMatrix();
    glTranslatef(-180,180,0);
    glRotatef(90,1,0,0);
    gluCylinder(gluNewQuadric(),20,20,150,100,100);
    drawCircle(20,0,0);
    glTranslatef(0,0,150);
    drawCircle(20,0,0);
    glPopMatrix();
}

void rightBackPaw()
{
    glColor3ub(81,68,61);
    glPushMatrix();
    glTranslatef(-200,50,0);
    glRotatef(90,0,1,0);
    gluCylinder(gluNewQuadric(),20,20,100,100,100);
    drawCircle(20,0,0);
    glTranslatef(0,0,100);
    drawCircle(20,0,0);
    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Torso
    torso_node.f = torso;
    torso_node.sibling = NULL;
    torso_node.child = &neck_node;
    glLoadIdentity();
    glRotatef(theta[0],0,0,1);
    glGetFloatv(GL_MODELVIEW_MATRIX,torso_node.m);

    //Neck
    neck_node.f = neck;
    neck_node.sibling = &lufl_node;
    neck_node.child = &head_node;
    glLoadIdentity();
    glTranslatef(220,300,-50);
    glRotatef(theta[1],0,0,1);
    glTranslatef(-220,-300,50);
    glGetFloatv(GL_MODELVIEW_MATRIX,neck_node.m);

    //Head
    head_node.f = head;
    head_node.sibling = NULL;
    head_node.child = NULL;
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX,head_node.m);

    //Creating Left Front Leg

    //Left Upper Front Leg
    lufl_node.f = leftUpperFrontLeg;
    lufl_node.sibling = &rufl_node;
    lufl_node.child = &llfl_node;
    glLoadIdentity();
    glTranslatef(220,270,-100);
    glRotatef(theta[2],0,0,1);
    glTranslatef(-220,-270,100);
    glGetFloatv(GL_MODELVIEW_MATRIX,lufl_node.m);

    //Left Lower Front Leg
    llfl_node.f = leftLowerFrontLeg;
    llfl_node.sibling = NULL;
    llfl_node.child = &lfp_node;
    glLoadIdentity();
    glTranslatef(220,180,-100);
    glRotatef(theta[3],0,0,1);
    glTranslatef(-220,-180,100);
    glGetFloatv(GL_MODELVIEW_MATRIX,llfl_node.m);

    //Left Front Paw
    lfp_node.f = leftFrontPaw;
    lfp_node.sibling = NULL;
    lfp_node.child = NULL;
    glLoadIdentity();
    glTranslatef(200,50,-100);
    glRotatef(theta[4],0,0,1);
    glTranslatef(-200,-50,100);
    glGetFloatv(GL_MODELVIEW_MATRIX,lfp_node.m);

    //Creating Right Front Leg

    glColor3ub(255,0,0);
    //Right Upper Front Leg
    rufl_node.f = rightUpperFrontLeg;
    rufl_node.sibling = &lubl_node;
    rufl_node.child = &rlfl_node;
    glLoadIdentity();
    glTranslatef(220,270,0);
    glRotatef(theta[5],0,0,1);
    glTranslatef(-220,-270,0);
    glGetFloatv(GL_MODELVIEW_MATRIX,rufl_node.m);

    //Right Lower Front Leg
    rlfl_node.f = rightLowerFrontLeg;
    rlfl_node.sibling = NULL;
    rlfl_node.child = &rfp_node;
    glLoadIdentity();
    glTranslatef(220,180,0);
    glRotatef(theta[6],0,0,1);
    glTranslatef(-220,-180,0);
    glGetFloatv(GL_MODELVIEW_MATRIX,rlfl_node.m);

    //Right Front Paw
    rfp_node.f = rightFrontPaw;
    rfp_node.sibling = NULL;
    rfp_node.child = NULL;
    glLoadIdentity();
    glTranslatef(200,50,0);
    glRotatef(theta[7],0,0,1);
    glTranslatef(-200,-50,0);
    glGetFloatv(GL_MODELVIEW_MATRIX,rfp_node.m);

    //Creating Left Back Leg

    //Left Upper Back Leg
    lubl_node.f = leftUpperBackLeg;
    lubl_node.sibling = &rubl_node;
    lubl_node.child = &llbl_node;
    glLoadIdentity();
    glTranslatef(-180,270,-100);
    glRotatef(theta[8],0,0,1);
    glTranslatef(180,-270,100);
    glGetFloatv(GL_MODELVIEW_MATRIX,lubl_node.m);

    //Left Lower Back Leg
    llbl_node.f = leftLowerBackLeg;
    llbl_node.sibling = NULL;
    llbl_node.child = &lbp_node;
    glLoadIdentity();
    glTranslatef(-180,180,-100);
    glRotatef(theta[9],0,0,1);
    glTranslatef(180,-180,100);
    glGetFloatv(GL_MODELVIEW_MATRIX,llbl_node.m);

    //Left Back Paw
    lbp_node.f = leftBackPaw;
    lbp_node.sibling = NULL;
    lbp_node.child = NULL;
    glLoadIdentity();
    glTranslatef(-200,50,-100);
    glRotatef(theta[10],0,0,1);
    glTranslatef(200,-50,100);
    glGetFloatv(GL_MODELVIEW_MATRIX,lbp_node.m);

    //Creating Right Back Leg

    //Right Upper Back Leg
    rubl_node.f = rightUpperBackLeg;
    rubl_node.sibling = NULL;
    rubl_node.child = &rlbl_node;
    glLoadIdentity();
    glTranslatef(-180,270,0);
    glRotatef(theta[11],0,0,1);
    glTranslatef(180,-270,0);
    glGetFloatv(GL_MODELVIEW_MATRIX,rubl_node.m);

    //Right Lower Back Leg
    rlbl_node.f = rightLowerBackLeg;
    rlbl_node.sibling = NULL;
    rlbl_node.child = &rbp_node;
    glLoadIdentity();
    glTranslatef(-180,180,0);
    glRotatef(theta[12],0,0,1);
    glTranslatef(180,-180,0);
    glGetFloatv(GL_MODELVIEW_MATRIX,rlbl_node.m);

    //Right Back Paw
    rbp_node.f = rightBackPaw;
    rbp_node.sibling = NULL;
    rbp_node.child = NULL;
    glLoadIdentity();
    glTranslatef(-200,50,0);
    glRotatef(theta[13],0,0,1);
    glTranslatef(200,-50,0);
    glGetFloatv(GL_MODELVIEW_MATRIX,rbp_node.m);

    glLoadIdentity();

    glRotatef(camera_theta,0,1,0);
    traverse(&torso_node);

    glFlush();
    glutSwapBuffers();
}

void animation1()
{
    target_degrees[5] = 20;
    target_degrees[6] = 100;
    target_degrees[7] = -180;
    max_degrees_in_animation = 180;
    for (int i = 5; i <= 7; i++)
    {
        if (target_degrees[i] > 0 && theta[i] < target_degrees[i])
            theta[i] += animation_speed * target_degrees[i] / max_degrees_in_animation;
        else if (target_degrees[i] < 0 && theta[i] > target_degrees[i])
            theta[i] -= animation_speed * -target_degrees[i] / max_degrees_in_animation;
        else
            play_animation = 0;
    }
}

void animation2()
{
    target_degrees[0] = 45;
    target_degrees[2] = -10;
    target_degrees[3] = 80;
    target_degrees[4] = -180;
    target_degrees[5] = -10;
    target_degrees[6] = 80;
    target_degrees[7] = -180;
    target_degrees[8] = 50;
    target_degrees[9] = -95;
    target_degrees[11] = 50;
    target_degrees[12] = -95;
    max_degrees_in_animation = 180;
    for (int i = 0; i < 14; i++)
    {
        if (i != 1 && i != 10 && i != 13)
        {
            if (target_degrees[i] > 0 && theta[i] < target_degrees[i])
                theta[i] += animation_speed * target_degrees[i] / max_degrees_in_animation;
            else if (target_degrees[i] < 0 && theta[i] > target_degrees[i])
                theta[i] -= animation_speed * -target_degrees[i] / max_degrees_in_animation;
            else
                play_animation = 0;
        }
    }
}

void animation3()
{
    target_degrees[1] = -45;
    max_degrees_in_animation = 45;
    if (theta[1] > target_degrees[1])
        theta[1] -= animation_speed;
    else
        play_animation = 0;
}

void animation4()
{
    for (int i = 0; i < 14; i++)
    {
        if (theta[i] > 0)
        {
            theta[i] -= animation_speed * fabs(target_degrees[i])/ max_degrees_in_animation;
            if (theta[i] < 0)
                theta[i] = 0;
        }
        else if (theta[i] < 0)
        {
            theta[i] += animation_speed * fabs(target_degrees[i]) / max_degrees_in_animation;
            if (theta[i] > 0)
                theta[i] = 0;
        }
    }
}

void menu(int id)
{
    if (!block_animation)
        play_animation = 1;

    if (id == 1)
    {
        animation_type = 1;
        block_animation = 1;
    }
    else if (id == 2)
    {
        animation_type = 2;
        block_animation = 1;
    }
    else if (id == 3)
    {
        animation_type = 3;
        block_animation = 1;
    }
    else if (id == 4)
    {
        animation_type = 4;
        block_animation = 0;
        play_animation = 1;
    }
    else
        exit(0);
}

void idle()
{
    if (play_animation == 1)
    {
        if (animation_type == 1)
            animation1();
        else if (animation_type == 2)
            animation2();
        else if (animation_type == 3)
            animation3();
        else if (animation_type == 4)
            animation4();
    }

    glutPostRedisplay();
}

//Use left and right arrows to rotate camera
void keyboard(int key, int x, int y){
    if (key == GLUT_KEY_LEFT)
        camera_theta += 1;
    else if (key == GLUT_KEY_RIGHT)
        camera_theta -= 1;
}

int main(int argc, char** argv) {

    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200,800);
    glutInitWindowPosition(400,100);
    glutCreateWindow("Dog Animation");
    glutDisplayFunc(display);
    initialize();
    display();
    glutSwapBuffers();

    glutCreateMenu(menu);
    glutAddMenuEntry("Lift and bend front leg",1);
    glutAddMenuEntry("Stand on back legs and bend front legs",2);
    glutAddMenuEntry("Move neck/head",3);
    glutAddMenuEntry("Return to neutral position",4);
    glutAddMenuEntry("Quit", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutIdleFunc(idle);
    glutSpecialFunc(keyboard);

    glutMainLoop();

    return 0;
}