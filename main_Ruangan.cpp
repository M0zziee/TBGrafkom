#include <GL/glut.h>
#include <cmath>
// Simbol Untuk Menentukan Kode Bagian Mana Yang Dicoding Oleh 
// M0ZZY = //***
// (Naufal) = //??   <-- ganti pakai nickname dan simbol tersendiri
// (Arief) = //??    <-- ganti pakai nickname dan simbol tersendiri

// Variabel untuk kontrol kamera
float cameraAngleX = 5.0f; 
float cameraAngleY = 1.0f;
float cameraDistance = 70.0f; 

int lastMouseX, lastMouseY; // Posisi terakhir kursor mouse
bool isDragging = false; // Status apakah mouse sedang digunakan
//***
void init() {
    glClearColor(0.5, 0.5, 0.5, 1.0); // Warna latar belakang abu-abu
    glEnable(GL_DEPTH_TEST);          // Mengaktifkan depth testing
}
//***
void drawCube(float x, float y, float z, float width, float height, float depth) {
  
  //*** Biar Tidak Riweh
  //*** Width = Lebar
  //*** Height = Tinggi
  //*** Depth = Kedalaman / Deep nickdeep ???
  
    glBegin(GL_QUADS);
    // Depan
    glVertex3f(x, y, z);
    glVertex3f(x + width, y, z);
    glVertex3f(x + width, y + height, z);
    glVertex3f(x, y + height, z);

    // Belakang
    glVertex3f(x, y, z - depth);
    glVertex3f(x + width, y, z - depth);
    glVertex3f(x + width, y + height, z - depth);
    glVertex3f(x, y + height, z - depth);

    // Kiri
    glVertex3f(x, y, z);
    glVertex3f(x, y, z - depth);
    glVertex3f(x, y + height, z - depth);
    glVertex3f(x, y + height, z);

    // Kanan
    glVertex3f(x + width, y, z);
    glVertex3f(x + width, y, z - depth);
    glVertex3f(x + width, y + height, z - depth);
    glVertex3f(x + width, y + height, z);

    // Atas
    glVertex3f(x, y + height, z);
    glVertex3f(x + width, y + height, z);
    glVertex3f(x + width, y + height, z - depth);
    glVertex3f(x, y + height, z - depth);

    // Bawah
    glVertex3f(x, y, z);
    glVertex3f(x + width, y, z);
    glVertex3f(x + width, y, z - depth);
    glVertex3f(x, y, z - depth);
    glEnd();
}
//***
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

    // Lantai
    glColor3f(0.6f, 0.4f, 0.2f); // Warna cokelat
    drawCube(-20.0f, 0.0f, 20.0f, 
              40.0f, 0.5f, 40.0f);

    // Dinding 1
    glColor3f(0.8f, 0.2f, 0.2f); // Warna merah
    drawCube(-20.0f, 0.5f, 20.0f,
              40.0f, 20.0f, 0.5f);

    // Dinding 2
    glColor3f(0.2f, 0.2f, 0.8f); // Warna biru
    drawCube(19.5f, 0.5f, 20.0f,
              0.5f, 20.0f, 40.0f);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}
//***
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
//***
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
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Dinding dan Lantai dengan Kontrol Kamera");
    glutFullScreen(); // biar fullscreen

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

