#include <GL/glut.h>
#include <math.h>

#define M_PI 3.14159265358979323846

/*
==========================================Global Variables==========================================
*/

//rotasi awan
struct Movement
{
	float rotate;
}
awan;
	
bool isMoving = true;

float awanPosX = 0.0f;  
float awanPosY = 0.0f; 
float awanPosZ = 0.0f; 

// Variable Untuk Posisi Kamera
float cameraAngleX = 5.0f;
float cameraAngleY = 1.0f;
float cameraDistance = 70.0f;

// Variabel untuk kontrol pencahayaan
float lightPosX = 10.0f;
float lightPosY = 10.0f;
float lightPosZ = 10.0f;

bool isSphereMode = false;
bool showLightCube = true; // Status untuk menampilkan kubus penanda cahaya


int lastMouseX, lastMouseY; // Posisi terakhir kursor mouse
bool isDragging = false;    // Status apakah mouse sedang digunakan
bool showAxes = false;      // Status untuk menampilkan garis kartesius

float progress = 0.0;

void initSecondsScene();
void initFirstScene();
int window_id = -1;
void changeScene(const char *tittle, void (*displayCallback)(void), void (*initCallback)(void));
void mainScene();
/*
==========================================Global Variables==========================================
*/


/*
==========================================BY NAUFAL==========================================
*/
void loadingScreen()
{
	glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3ub(255, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2d(-800, -5);
    glVertex2d(-800, 5);
    glVertex2d(800, 5);
    glVertex2d(800, -5);
    glEnd();

    float bar = -800 + (800 * (progress * 2));
    glBegin(GL_QUADS);
    glVertex2d(-800, -5);
    glVertex2d(-800, 5);
    glVertex2d(bar, 5);
    glVertex2d(bar, -5);
    glEnd();
    
   	glutSwapBuffers();
	
    if (progress < 1.0)
    {
        progress += 0.001;
        glutPostRedisplay();
    }
    else
    {
        changeScene("PIRADMID", mainScene, initSecondsScene);
    }
}
/*
==========================================BY NAUFAL==========================================
*/


/*
==========================================BY FATHIR==========================================
*/
void init()
{
	
    glEnable(GL_COLOR_MATERIAL);                                    // Aktifkan material warna
    glClearColor(132.0 / 255.0, 198.0 / 255.0, 227.0 / 255.0, 1.0); // Warna latar belakang abu-abu
    glEnable(GL_DEPTH_TEST);                                        // Mengaktifkan depth testing
    glEnable(GL_LIGHTING);                                          // Aktifkan pencahayaan
    glEnable(GL_LIGHT0);                                            // Aktifkan sumber cahaya 0
    glEnable(GL_NORMALIZE);                                         // Normalisasi norma permukaan

    // Atur sumber cahaya
    GLfloat lightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
}
/*
==========================================BY FATHIR==========================================
*/

/*
==========================================BY FATHIR==========================================
*/
void drawCylinder(float radius, float height, int slices)
{
    float x, y, z, angle;

    // Draw the tube
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++)
    {
        angle = 2.0f * M_PI * (float)i / (float)slices;
        x = radius * cosf(angle);
        z = radius * sinf(angle);

        glNormal3f(x / radius, 0.0f, z / radius);
        glVertex3f(x, 0.0f, z);
        glVertex3f(x, height, z);
    }
    glEnd();

    // Draw the top and bottom circles
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f);
    for (int i = slices; i >= 0; i--)
    {
        angle = 2.0f * M_PI * (float)i / (float)slices;
        x = radius * cosf(angle);
        z = radius * sinf(angle);
        glVertex3f(x, height, z);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f);
    for (int i = 0; i <= slices; i++)
    {
        angle = 2.0f * M_PI * (float)i / (float)slices;
        x = radius * cosf(angle);
        z = radius * sinf(angle);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();
}

void drawCactus(float x, float y, float z ,float size, float r) 
{
    glPushMatrix();

    // Posisikan kaktus
    glTranslatef(x, y, z);
	glScalef(size,size,size);
	glRotatef(x,y,z,r);
    // Batang utama
    drawCylinder(0.5f, 5.0f, 32);

    // Cabang kanan
    glPushMatrix();
    glTranslatef(0.0f, 3.0f, 0.0f);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    drawCylinder(0.35f, 2.0f, 32);
    glPopMatrix();

    // Cabang kiri
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);
    drawCylinder(0.35f, 2.0f, 32);
    glPopMatrix();

    glPopMatrix();
}

void drawAxes()
{
    glDisable(GL_LIGHTING); // Matikan pencahayaan untuk sumbu
    glBegin(GL_LINES);

    // Sumbu X (merah)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-100.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);

    // Sumbu Y (hijau)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -100.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);

    // Sumbu Z (biru)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -100.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);

    glEnd();
    glEnable(GL_LIGHTING); // Aktifkan kembali pencahayaan
}
/*
==========================================BY FATHIR==========================================
*/

/*
==========================================BY NAUFAL==========================================
*/

void drawAwan(float x,float y,float z) 
{ 
  glPushMatrix();
     glTranslatef(x + awanPosX, y + awanPosY, z + awanPosZ);
    
 
    glColor3f(1.0f, 1.0f, 1.0f);  // Warna putih untuk awan
   	glRotated(awan.rotate, 0.0, 1.0, 0.0);
    glutSolidSphere(4.0, 20, 20); // Sphere pertama

    glPushMatrix();
    glTranslatef(5.0f, 0.0f, 0.0f);
    glRotated(awan.rotate, 0.0, 1.0, 0.0);
    glutSolidSphere(3.0, 20, 20); // Sphere kedua
    glPopMatrix();

    glPushMatrix();
   	glTranslatef(-5.0f, 0.0f, 0.0f);
    glRotated(awan.rotate, 0.0, 1.0, 0.0);
    glutSolidSphere(3.0f, 20, 20); // Sphere ketiga
    glPopMatrix();
	
    glPopMatrix();
}

void drawCube()
{ 
    glPushMatrix();
    glTranslatef(0.0f, -15.0f, 0.0f); // Posisikan kubus di bawah oktahedron
    glScalef(-70.0f, 30.0f, 80.0f);   // Sesuaikan ukuran kubus
    glutSolidCube(1.0f);
    glPopMatrix();
}
/*
==========================================BY NAUFAL==========================================
*/

/*
==========================================BY ARIEF==========================================
*/
void drawLight()
{
    glDisable(GL_LIGHTING); // Matikan pencahayaan untuk bola matahari
    glPushMatrix();

    // Posisikan bola matahari di lokasi cahaya
    glTranslatef(lightPosX, lightPosY, lightPosZ);

    // Atur warna berdasarkan mode
    if (isNightMode)
    {
        glColor3f(1.0f, 1.0f, 1.0f); // Warna putih untuk mode malam
    }
    else
    {
        glColor3f(1.0f, 0.7f, 0.0f); // Warna oranye untuk mode siang
    }

    glutSolidSphere(5.0, 20, 20); // Menggambar bola sebagai representasi cahaya

    glPopMatrix();
    glEnable(GL_LIGHTING); // Aktifkan kembali pencahayaan
}

void processMenu(int option)
{
	switch(option)
	{
	case 1:
        	isSphereMode = false;
		glClearColor(132.0f / 255.0f, 198.0f / 255.0f, 227.0f / 255.0f, 1.0);
	break;
	case 2:
        	isSphereMode = true; 
        	glClearColor(0.1f, 0.1f, 0.2f, 1.0);
		break;
	case 3:
		isMoving = !isMoving;
		break;
	case 4:
		exit(0);
	}
	glutPostRedisplay();
}

void createMenu() {
    glutCreateMenu(processMenu);
    glutAddMenuEntry("Mode Siang", 1);
    glutAddMenuEntry("Mode Malam", 2);
    glutAddMenuEntry("Pegerakan Awan",3);
    glutAddMenuEntry("Keluar", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
/*
==========================================BY ARIEF==========================================
*/

/*
==========================================BY FATHIR==========================================
*/
void mainScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

    // Atur kamera
    float cameraPosX = cameraDistance * sinf(cameraAngleX) * cosf(cameraAngleY);
    float cameraPosY = cameraDistance * sinf(cameraAngleY);
    float cameraPosZ = cameraDistance * cosf(cameraAngleX) * cosf(cameraAngleY);
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ, // Posisi kamera
              0.0, 0.0, 0.0,                      // Titik fokus
              0.0, 1.0, 0.0);                     // Arah atas

    // Atur posisi pencahayaan
    GLfloat lightPosition[] = {lightPosX, lightPosY, lightPosZ, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    // Tambahkan material
    GLfloat materialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Warna putih
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

    glPushMatrix();
    glScalef(20.0f, 20.0f, 20.0f); // Perbesar oktahedron
    glColor3ub(195, 154, 108);     // Warna putih untuk oktahedron
    glutSolidOctahedron();
    glPopMatrix();

    glColor3ub(234, 203, 164);
    drawCube(); // pemanggilan function cube
    glColor3ub(86, 115, 46);
    drawCactus(10, 0, 20,1,0); // panggil kaktus
    drawCactus(-20, 0, -20,1,0);
    drawCactus(-20, 0, 10,1,0);
    drawCactus(-13, 0, 10,1,0);
    drawCactus(17, 0, -10,1,0);
    drawCactus(17, 0, 10,1,0);
    drawCactus(14, 0, 12,1,0);
    
    //awan by naufal
    glPushMatrix();
  
    glScalef(2.0f,1.0f,2.0f);
    glColor3ub(244, 244, 244);
    drawAwan(0, 24, 10); 
    drawAwan(0 ,25 ,9);
    drawAwan(10, 24, 10); 
    drawAwan(4, 24, 10); 
    drawAwan(7 ,24 ,9);
    glPopMatrix();
    
    glPushMatrix();
	glScalef(2.0f, 1.0f, 2.0f);
	glColor3ub(244, 244, 244);

	// Membalikkan posisi X dan Z
	drawAwan(-0, 24, 10);  // Berkebalikan di sumbu X
	drawAwan(0 ,25 ,9);
	drawAwan(-10, 24, 10); // Berkebalikan di sumbu X dan Z
	drawAwan(-4, 24, 10);  // Berkebalikan di sumbu X
	drawAwan(-7 ,24 ,9);   // Berkebalikan di sumbu X

	glPopMatrix();



    if (showAxes)
    {
        drawAxes();
    }

    if (showLightCube)
    {
        drawLight();
    }

    glutSwapBuffers();
}
  
void reshape(int w, int h) //resize window agar tidak streched
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

// Fungsi untuk menangani pergerakan mouse
void mouseMotion(int x, int y)
{
    if (isDragging)
    {
        // Hitung perubahan posisi mouse
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;

        // Perbarui sudut kamera berdasarkan perubahan mouse
        cameraAngleX += dx * 0.005f;
        cameraAngleY += dy * 0.005f;

        // Batasi sudut kamera agar tidak melebihi batas
        if (cameraAngleY > M_PI / 2.0f)
            cameraAngleY = M_PI / 2.0f;
        if (cameraAngleY < -M_PI / 2.0f)
            cameraAngleY = -M_PI / 2.0f;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay(); // Render ulang
    }
}

// Fungsi untuk menangani klik mouse
void mouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            isDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else if (state == GLUT_UP)
        {
            isDragging = false;
        }
    }
}

// Fungsi untuk menangani input keyboard
void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
    { // ESC
        exit(0);
    }
    else if (key == 'c' || key == 'C')
    { // Tombol untuk menampilkan garis kartesius
        showAxes = !showAxes;
        glutPostRedisplay();
    }
    else if (key == 'w' || key == 'W')
    { // Gerakkan cahaya ke atas
        lightPosY += 1.0f;
        glutPostRedisplay();
    }
    else if (key == 's' || key == 'S')
    { // Gerakkan cahaya ke bawah
        lightPosY -= 1.0f;
        glutPostRedisplay();
    }
    else if (key == 'a' || key == 'A')
    { // Gerakkan cahaya ke kiri
        lightPosX -= 1.0f;
        glutPostRedisplay();
    }
    else if (key == 'd' || key == 'D')
    { // Gerakkan cahaya ke kanan
        lightPosX += 1.0f;
        glutPostRedisplay();
    }
    else if (key == 'q' || key == 'Q')
    { // Gerakkan cahaya ke depan
        lightPosZ += 1.0f;
        glutPostRedisplay();
    }
    else if (key == 'e' || key == 'E')
    { // Gerakkan cahaya ke belakang
        lightPosZ -= 1.0f;
        glutPostRedisplay();
    }
    else if (key == 'l' || key == 'L')
    { // Tombol untuk menampilkan kubus penanda cahaya
        showLightCube = !showLightCube;
        glutPostRedisplay();
    }
    else if (key == 'f' || key == 'F')
    { // Gerakkan awab ke kiri
        awanPosX -= 1.0f;
        glutPostRedisplay();
    }
    else if (key == 't' || key == 'T')
    { // Gerakkan awan ke depan
        awanPosZ += 1.0f;
        glutPostRedisplay();
    }
    else if (key == 'h' || key == 'H')
    { // Gerakkan cahaya ke kanan
        awanPosX += 1.0f;
        glutPostRedisplay();
    }
    else if (key == 'g' || key == 'G')
    { // gerakakan awan ke belakang
        awanPosZ -= 1.0f;
        glutPostRedisplay();
    }
   
}
/*
==========================================BY FATHIR==========================================
*/

/*
==========================================BY NAUFAL==========================================
*/
void initFirstScene()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-100, 100, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

void initSecondsScene()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, 1.0, 1.0, 100.0);
    gluLookAt(10.0, 10.0, 17.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.1, 0.1, 0.1, 1.0);
}

void changeScene(const char *title, void (*displayCallback)(void), void (*initCallback)(void))
{
    if (window_id != -1)
        glutDestroyWindow(window_id);
    window_id = glutCreateWindow(title);
    glutDisplayFunc(displayCallback);
    initCallback();
    init();
    createMenu();
  
    glutReshapeFunc(reshape);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouseClick); 
    glutKeyboardFunc(keyboard);
    glutFullScreen();
}

void update(int value)
{

{
	
		if(isMoving)
		{
		
			awan.rotate += 0.1;
				
	}
		glutPostRedisplay();
		
	}
	glutTimerFunc(1000 / 60, update, 0);
	
}
/*
==========================================BY NAUFAL==========================================
*/

/*
==========================================BY FATHIR==========================================
*/
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    changeScene("loading....", loadingScreen, initFirstScene);  
	glutTimerFunc(1000 / 60, update, 0);
    glutMainLoop();
    return 0;
}
/*
==========================================BY FATHIR==========================================
*/
