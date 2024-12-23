#include <GL/glut.h>
#include <cmath>

// Variabel untuk kontrol kamera
float cameraAngleX = 5.0f; 
float cameraAngleY = 1.0f;
float cameraDistance = 70.0f; 

// Variabel untuk kontrol pencahayaan
float lightPosX = 10.0f;
float lightPosY = 10.0f;
float lightPosZ = 10.0f;
bool showLightCube = true; // Status untuk menampilkan kubus penanda cahaya

int lastMouseX, lastMouseY; // Posisi terakhir kursor mouse
bool isDragging = false; // Status apakah mouse sedang digunakan
bool showAxes = false; // Status untuk menampilkan garis kartesius

void init() {
    glClearColor(0.5, 0.5, 0.5, 1.0); // Warna latar belakang abu-abu
    glEnable(GL_DEPTH_TEST);          // Mengaktifkan depth testing
    glEnable(GL_LIGHTING);            // Aktifkan pencahayaan
    glEnable(GL_LIGHT0);              // Aktifkan sumber cahaya 0

    // Atur sumber cahaya
    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
}

void drawAxes() {
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

void drawLightCube() {
    glDisable(GL_LIGHTING); // Matikan pencahayaan untuk kubus
    glPushMatrix();
    glTranslatef(lightPosX, lightPosY, lightPosZ);
    glColor3f(1.0f, 1.0f, 0.0f); // Warna kuning untuk kubus
    glutSolidCube(1.0f);
    glPopMatrix();
    glEnable(GL_LIGHTING); // Aktifkan kembali pencahayaan
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Atur kamera
    float cameraPosX = cameraDistance * sinf(cameraAngleX) * cosf(cameraAngleY);
    float cameraPosY = cameraDistance * sinf(cameraAngleY);
    float cameraPosZ = cameraDistance * cosf(cameraAngleX) * cosf(cameraAngleY);
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ,  // Posisi kamera
              0.0, 0.0, 0.0,                      // Titik fokus
              0.0, 1.0, 0.0);                     // Arah atas

    // Atur posisi pencahayaan
    GLfloat lightPosition[] = { lightPosX, lightPosY, lightPosZ, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glPushMatrix();
    glScalef(20.0f, 20.0f, 20.0f); // Perbesar oktahedron
    glColor3f(1.0f, 1.0f, 1.0f);   // Warna putih untuk oktahedron
    glutSolidOctahedron();
    glPopMatrix();

    if (showAxes) {
        drawAxes();
    }

    if (showLightCube) {
        drawLightCube();
    }

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Fungsi untuk menangani pergerakan mouse
void mouseMotion(int x, int y) {
    if (isDragging) {
        // Hitung perubahan posisi mouse
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;

        // Perbarui sudut kamera berdasarkan perubahan mouse
        cameraAngleX += dx * 0.005f;
        cameraAngleY += dy * 0.005f;

        // Batasi sudut kamera agar tidak melebihi batas
        if (cameraAngleY > M_PI / 2.0f) cameraAngleY = M_PI / 2.0f;
        if (cameraAngleY < -M_PI / 2.0f) cameraAngleY = -M_PI / 2.0f;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay(); // Render ulang
    }
}

// Fungsi untuk menangani klik mouse
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            isDragging = false;
        }
    }
}

// Fungsi untuk menangani input keyboard
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC
        exit(0);
    } else if (key == 'c' || key == 'C') { // Tombol untuk menampilkan garis kartesius
        showAxes = !showAxes;
        glutPostRedisplay();
    } else if (key == 'w' || key == 'W') { // Gerakkan cahaya ke atas
        lightPosY += 1.0f;
        glutPostRedisplay();
    } else if (key == 's' || key == 'S') { // Gerakkan cahaya ke bawah
        lightPosY -= 1.0f;
        glutPostRedisplay();
    } else if (key == 'a' || key == 'A') { // Gerakkan cahaya ke kiri
        lightPosX -= 1.0f;
        glutPostRedisplay();
    } else if (key == 'd' || key == 'D') { // Gerakkan cahaya ke kanan
        lightPosX += 1.0f;
        glutPostRedisplay();
    } else if (key == 'q' || key == 'Q') { // Gerakkan cahaya ke depan
        lightPosZ += 1.0f;
        glutPostRedisplay();
    } else if (key == 'e' || key == 'E') { // Gerakkan cahaya ke belakang
        lightPosZ -= 1.0f;
        glutPostRedisplay();
    } else if (key == 'l' || key == 'L') { // Tombol untuk menampilkan kubus penanda cahaya
        showLightCube = !showLightCube;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("PYRAMID");
	glutFullScreen();
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
