#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/


// remember to prototype
void generateGeometry(void);
void initGPUBuffers(void);
void shaderSetup(void);
void display(void);
void keyboard(unsigned char key, int x, int y);

// Xác định kiểu dữ liệu
typedef vec4 point4;
typedef vec4 color4;
using namespace std;

GLuint program;
GLuint model_loc;
GLuint projection_loc;
GLuint view_loc;

mat4 model, modelchanban2;
mat4 projection;
mat4 view;

// Hình lập phương
const int soDinhLapPhuong = 36;

// Danh sách các đỉnh của các tam giác cần vẽ
point4 arrDinhTamGiac[soDinhLapPhuong];
// Danh sách các màu tương ứng cho các đỉnh trên
color4 arrMauCacDinh[soDinhLapPhuong];
//Danh sách các vector pháp tuyến ứng với mỗi đỉnh
vec3 arrVecPhapTuyen[soDinhLapPhuong];

// Danh sách 8 đỉnh của hình lập phương
point4 cacDinhLapPhuong[8];
// Danh sách các màu cho 8 đỉnh hình lập phương
color4 cacMauHinhLapPhuong[8];


// Vị trí ánh sáng
//point4 viTriAnhSang(0.025, 3, 5, 1.0);
point4 viTriAnhSang(0.025, 10, 5, 1.0);
// Màu gốc ánh sáng
color4 mauAnhSang(1, 1, 1, 1.0);
// Màu khuếch tán
color4 mauAnhSangKhuechTan(1.0, 1.0, 1.0, 1.0);
// Màu gương phản xạ lại
color4 mauAnhSangPhanXaGuong(1.0, 1.0, 1.0, 1.0);

// Màu gốc vật liệu
color4 mauVatLieu;
// Màu khuếch tán vật liệu
color4 mauVatLieuKhuechTan;
// Màu phản xạ gương vật liệu
color4 mauVatLieuPhanXaGuong;
// Độ bóng vật liệu
float doBongVatLieu = 1000.0;

// Màu được trộn từ 2 màu gốc
color4 mauGocCau;
// Màu khuếch tán được trộn từ 2 màu gốc
color4 mauKhuechTanCau;
// Màu phản xạ gương được trộn từ 2 màu gốc
color4 mauPhanXaGuongCau;

void HinhLapPhuong()
{
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	cacDinhLapPhuong[0] = point4(-0.5, -0.5, 0.5, 1.0);
	cacDinhLapPhuong[1] = point4(-0.5, 0.5, 0.5, 1.0);
	cacDinhLapPhuong[2] = point4(0.5, 0.5, 0.5, 1.0);
	cacDinhLapPhuong[3] = point4(0.5, -0.5, 0.5, 1.0);
	cacDinhLapPhuong[4] = point4(-0.5, -0.5, -0.5, 1.0);
	cacDinhLapPhuong[5] = point4(-0.5, 0.5, -0.5, 1.0);
	cacDinhLapPhuong[6] = point4(0.5, 0.5, -0.5, 1.0);
	cacDinhLapPhuong[7] = point4(0.5, -0.5, -0.5, 1.0);

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	cacMauHinhLapPhuong[0] = color4(0.0, 0.0, 0.0, 1.0); // đen
	cacMauHinhLapPhuong[1] = color4(1.0, 0.0, 0.0, 1.0); // đỏ
	cacMauHinhLapPhuong[2] = color4(1.0, 1.0, 0.0, 1.0); // vàng
	cacMauHinhLapPhuong[3] = color4(0.0, 1.0, 0.0, 1.0); // xanh lá
	cacMauHinhLapPhuong[4] = color4(0.0, 0.0, 1.0, 1.0); // xanh lam
	cacMauHinhLapPhuong[5] = color4(1.0, 0.0, 1.0, 1.0); // tím
	cacMauHinhLapPhuong[6] = color4(1.0, 0.5, 0.0, 1.0); // cam
	cacMauHinhLapPhuong[7] = color4(0.0, 1.0, 1.0, 1.0); // xanh lơ
}


int Index = 0;
// Tạo một mặt hình lập phương (đưa đỉnh, pháp tuyến, màu vào mảng thứ tự cho việc chuyển đến bộ nhớ, )
void TaoMotMatHLP(int a, int b, int c, int d)
{
	vec4 u = cacDinhLapPhuong[b] - cacDinhLapPhuong[a];
	vec4 v = cacDinhLapPhuong[c] - cacDinhLapPhuong[b];
	vec3 normal = normalize(cross(u, v));

	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[a]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[b]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[c]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[a]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[c]; Index++;
	arrVecPhapTuyen[Index] = normal; arrMauCacDinh[Index] = cacMauHinhLapPhuong[a]; arrDinhTamGiac[Index] = cacDinhLapPhuong[d]; Index++;
}

void TaoHinhVuong()  /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/
{
	TaoMotMatHLP(1, 0, 3, 2);
	TaoMotMatHLP(2, 3, 7, 6);
	TaoMotMatHLP(3, 0, 4, 7);
	TaoMotMatHLP(6, 5, 1, 2);
	TaoMotMatHLP(4, 5, 6, 7);
	TaoMotMatHLP(5, 4, 0, 1);
}

void generateGeometry(void)
{
	HinhLapPhuong();
	TaoHinhVuong();
}

// Một số phương thức màu sắc, góc
float RGBConvert(float _param)
{
	return _param / 255.0;
}
color4 RGBtoColor(float _R, float _G, float _B)
{
	return color4(RGBConvert(_R), RGBConvert(_G), RGBConvert(_B), 1.0);
}
float DEGtoRAD(float DEG)
{
	return DEG / 360 * 2 * M_PI;
}

void CauMau()
{
	//  sử mô hình blinn - Phong
	mauGocCau = mauAnhSang * mauVatLieu;
	mauKhuechTanCau = mauAnhSangKhuechTan * mauVatLieuKhuechTan;
	mauPhanXaGuongCau = mauAnhSangPhanXaGuong * mauVatLieuPhanXaGuong;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, mauGocCau);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, mauKhuechTanCau);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, mauPhanXaGuongCau);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, viTriAnhSang);
	glUniform1f(glGetUniformLocation(program, "Shininess"), doBongVatLieu);
}
GLfloat ti_le_anh_sang = 0.8;
void TaoVatLieu(color4 mauGoc, color4 mauPhanXa, color4 mauPXGuong, float doBong)
{
	// Nhập 3 màu vào và trộn màu cho vật thể
	mauVatLieu = mauGoc * ti_le_anh_sang;
	mauVatLieuKhuechTan = mauPhanXa;
	mauVatLieuPhanXaGuong = mauPXGuong;
	doBongVatLieu = doBong;
	CauMau();
}

// Hàm vẽ hình lập phương tâm O,  với ma trận _matrix * _kichthuoc
void VeHinhLapPhuong(mat4 _kichthuoc, mat4 _matrix)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * _matrix * _kichthuoc);
	glDrawArrays(GL_TRIANGLES, 0, soDinhLapPhuong);
}

void initGPUBuffers(void)
{
	// Tạo một VAO - vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Tạo và khởi tạo một buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(arrDinhTamGiac) + sizeof(arrMauCacDinh) + sizeof(arrVecPhapTuyen), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(arrDinhTamGiac), arrDinhTamGiac);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(arrDinhTamGiac), sizeof(arrMauCacDinh), arrMauCacDinh);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(arrDinhTamGiac) + sizeof(arrMauCacDinh), sizeof(arrVecPhapTuyen), arrVecPhapTuyen);
}

void shaderSetup(void)
{
	// Nạp các shader và sử dụng chương trình shader
	program = InitShader("vshader1.glsl", "fshader1.glsl");   // hàm InitShader khai báo Caug Angel.h
	glUseProgram(program);

	// Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
	GLuint loc_vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc_vPosition);
	glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(arrDinhTamGiac)));

	GLuint loc_vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(loc_vNormal);
	glVertexAttribPointer(loc_vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(arrDinhTamGiac) + sizeof(arrMauCacDinh)));

	model_loc = glGetUniformLocation(program, "Model");
	projection_loc = glGetUniformLocation(program, "Projection");
	view_loc = glGetUniformLocation(program, "View");

	glEnable(GL_DEPTH_TEST);
}
//màu sắc
#define BLACK 0
#define RED 1
#define YELLOW 2
#define GREEN 3
#define BLUE 4
#define MAGENTA 5
#define WHITE 6
#define CYAN 7
#define ORANGE 8
#define BROWN 9
#define GRAY 10
#define DARKGREEN 11
#define PINK 12
#define ROAD 13
#define MATDAT 14

//vật liệu
#define NHUA 0
#define DONG 1
#define BAC 2

//chiếu sáng
color4 mauGoc[100];
color4 mauPhanXa[10];
color4 mauPXGuong[10];
GLfloat doBong[10];
void setup_color() {
	mauGoc[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	mauGoc[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	mauGoc[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	mauGoc[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	mauGoc[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
	mauGoc[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
	mauGoc[6] = color4(1.0, 1.0, 1.0, 1.0); // white
	mauGoc[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
	mauGoc[8] = color4(1.0, 0.6, 0.0, 1.0); // orange
	mauGoc[9] = color4(0.3, 0.0, 0.0, 1.0); // brown
	mauGoc[10] = RGBtoColor(220, 220, 220); // gray
	mauGoc[11] = RGBtoColor(0, 100, 0); //dark green
	mauGoc[12] = RGBtoColor(219, 112, 147); //pink
	mauGoc[13] = RGBtoColor(128, 128, 128); //more gray
	mauGoc[14] = RGBtoColor(150, 92, 92); //more gray
	//---------------------
	mauPhanXa[0] = color4(0.01, 0.01, 0.01, 1.0); // nhựa
	mauPhanXa[1] = color4(0.780392, 0.568627, 0.113725, 1.0); // đồng
	mauPhanXa[2] = color4(0.2775, 0.2775, 0.2775, 1.0); // bạc
	//---------------------
	mauPXGuong[0] = color4(0.5, 0.5, 0.5, 1); //nhựa
	mauPXGuong[1] = color4(0.992157, 0.941176, 0.807843, 1.0); // đồng
	mauPXGuong[2] = color4(0.773911, 0.773911, 0.773911, 1.0); // bạc
	//-----------------------
	doBong[0] = 32; // nhựa
	doBong[1] = 27.8974; // đồng
	doBong[2] = 89.6; // bạc
	doBong[3] = 0;
}

void TaoVatLieuVer2(GLuint mau, GLuint vatLieu) {
	TaoVatLieu(mauGoc[mau], mauPhanXa[vatLieu], mauPXGuong[vatLieu], doBong[vatLieu]);
}

mat4 view_mat;
mat4 move_mat;
vec4 eye(2.5, 1.6, -6, 1), at(0, 0, 0, 1), up(0, 1, 0, 1);
GLfloat l = -0.01, r = 0.01, b = -0.01, t = 0.01, zN = 0.02, zF = 50;
GLfloat matXY[2]; // dọc ngang
vec4 mat_dinhhuong(0, 0, 1, 1);

void ngoai_canh()
{
	glClearColor(0.53 * ti_le_anh_sang, 0.8 * ti_le_anh_sang, 0.92 * ti_le_anh_sang, 1.0); // màu bầu trời
	model = mat4();
	TaoVatLieuVer2(PINK, NHUA);

	//nhà
	//Tầng 1
	VeHinhLapPhuong(Scale(0.1, 4.0, 10), Translate(-0.05, 2, 5));
	VeHinhLapPhuong(Scale(0.1, 4.0, 10), Translate(5.05, 2, 5));
	VeHinhLapPhuong(Scale(5, 0.1, 10), Translate(2.5, 4.05, 5));
	VeHinhLapPhuong(Scale(5, 4.0, 0.1), Translate(2.5, 2, 9.95));
	//sàn
	TaoVatLieuVer2(GRAY, NHUA);
		VeHinhLapPhuong(Scale(5, 0.1, 10), Translate(2.5, 0.05, 5));
	//tường
	TaoVatLieuVer2(PINK, NHUA);
	VeHinhLapPhuong(Scale(3.5, 4.0, 0.1), Translate(1.75, 2.0, 6.05));
	//Tầng 2
	TaoVatLieuVer2(PINK, NHUA);
	VeHinhLapPhuong(Scale(0.1, 4.0, 10), Translate(-0.05, 6.0, 5));
	VeHinhLapPhuong(Scale(0.1, 4.0, 10), Translate(5.05, 6.0, 5));
	VeHinhLapPhuong(Scale(5, 0.1, 10), Translate(2.5, 7.95, 5));
	VeHinhLapPhuong(Scale(5, 4.0, 0.1), Translate(2.5, 6.0, 9.95));

	//Cửa
	VeHinhLapPhuong(Scale(3.5, 4.0, 0.1), Translate(1.75, 6.0, 1.05));
	VeHinhLapPhuong(Scale(1.0, 2, 0.1), Translate(4.0, 7.25, 1.05));
	VeHinhLapPhuong(Scale(0.5, 4.0, 0.1), Translate(4.75, 6.0, 1.05));

	//Ban công
		//Khung
	TaoVatLieuVer2(GRAY, NHUA);
	VeHinhLapPhuong(Scale(5, 0.1, 0.1), Translate(2.5, 5.15, 0.05));
	//Cột
	VeHinhLapPhuong(Scale(0.05, 1, 0.1), Translate(0.0, 4.6, 0.05));
	VeHinhLapPhuong(Scale(0.1, 1, 0.1), Translate(0.5, 4.6, 0.05));
	VeHinhLapPhuong(Scale(0.1, 1, 0.1), Translate(1.0, 4.6, 0.05));
	VeHinhLapPhuong(Scale(0.1, 1, 0.1), Translate(1.5, 4.6, 0.05));
	VeHinhLapPhuong(Scale(0.1, 1, 0.1), Translate(2.0, 4.6, 0.05));
	VeHinhLapPhuong(Scale(0.1, 1, 0.1), Translate(2.5, 4.6, 0.05));
	VeHinhLapPhuong(Scale(0.1, 1, 0.1), Translate(3.0, 4.6, 0.05));
	VeHinhLapPhuong(Scale(0.1, 1, 0.1), Translate(3.5, 4.6, 0.05));
	VeHinhLapPhuong(Scale(0.1, 1, 0.1), Translate(4.0, 4.6, 0.05));
	VeHinhLapPhuong(Scale(0.1, 1, 0.1), Translate(4.5, 4.6, 0.05));
	VeHinhLapPhuong(Scale(0.05, 1, 0.1), Translate(5.0, 4.6, 0.05));

	//mặt đất
	TaoVatLieuVer2(MATDAT, NHUA);
	VeHinhLapPhuong(Scale(100, 0.1, 20), Translate(2.5, -0.05, 10));
	VeHinhLapPhuong(Scale(100, 0.1, 50), Translate(2.5, -0.05, -35));
	VeHinhLapPhuong(Scale(49, 0.1, 4), Translate(-23, -0.05, -2));
	VeHinhLapPhuong(Scale(49, 0.1, 4), Translate(28, -0.05, -2));

	//đường
	TaoVatLieuVer2(ROAD, NHUA);
	VeHinhLapPhuong(Scale(2, 0.1, 4), Translate(2.5, -0.05, -2));
	VeHinhLapPhuong(Scale(100, 0.1, 6), Translate(2.5, -0.05, -7));


	//ngoại cảnh
	TaoVatLieuVer2(RED, NHUA);
	VeHinhLapPhuong(Scale(5.0, 4.0, 10), Translate(9.5, 2.0, 5.05));
	TaoVatLieuVer2(GREEN, NHUA);
	VeHinhLapPhuong(Scale(5.0, 8.0, 10), Translate(16.0, 4.0, 5.05));
	TaoVatLieuVer2(BLUE, NHUA);
	VeHinhLapPhuong(Scale(5.0, 8.0, 10), Translate(-4.5, 4.0, 5.05));
	TaoVatLieuVer2(GREEN, NHUA);
	VeHinhLapPhuong(Scale(5.0, 4.0, 10), Translate(-11, 2.0, 5.05));
}

//cửa cuốn
GLuint cua_cuon_status = 0;
GLfloat cua_cuon_do_dai = 4.1;
void cua_cuon()
{
	model = mat4();
	TaoVatLieuVer2(GRAY, NHUA);
	VeHinhLapPhuong(Scale(5, cua_cuon_do_dai, 0.05), Translate(2.5, 4.1 - cua_cuon_do_dai / 2, 0.025));
}

//đồng hồ
void bagio() {
	VeHinhLapPhuong(Scale(0.01, 0.05, 0.01), Translate(-0.03, 0, -0.2 + -0.02));
	VeHinhLapPhuong(Scale(0.01, 0.05, 0.01), Translate(-0.03, 0, -0.2 + 0.));
	VeHinhLapPhuong(Scale(0.01, 0.05, 0.01), Translate(-0.03, 0, -0.2 + 0.02));
}
void muoihaigio() {
	VeHinhLapPhuong(Scale(0.01, 0.05, 0.01), Translate(-0.03, 0.2, -0.04));
	VeHinhLapPhuong(Scale(0.01, 0.05, 0.01), Translate(-0.03, 0.2, -0.02));
	VeHinhLapPhuong(Scale(0.01, 0.06, 0.01), Translate(-0.03, 0.2, +0.02) * RotateX(45));
	VeHinhLapPhuong(Scale(0.01, 0.06, 0.01), Translate(-0.03, 0.2, +0.02) * RotateX(-45));
}
void saugio() {
	VeHinhLapPhuong(Scale(0.01, 0.05, 0.01), Translate(-0.03, -0.2, -0.03));
	VeHinhLapPhuong(Scale(0.01, 0.05, 0.01), Translate(-0.03, -0.2, 0) * RotateX(-30));
	VeHinhLapPhuong(Scale(0.01, 0.05, 0.01), Translate(-0.03, -0.2, 0.02) * RotateX(30));
}
void chingio() {
	VeHinhLapPhuong(Scale(0.01, 0.05, 0.01), Translate(-0.03, 0, 0.2 + 0.02));
	VeHinhLapPhuong(Scale(0.01, 0.06, 0.01), Translate(-0.03, 0, 0.2 - 0.02) * RotateX(45));
	VeHinhLapPhuong(Scale(0.01, 0.06, 0.01), Translate(-0.03, 0, 0.2 - 0.02) * RotateX(-45));
}
void kimgio() {
	TaoVatLieuVer2(RED, NHUA);
	VeHinhLapPhuong(Scale(0.01, 0.12, 0.01), Translate(-0.03, 0.06, 0));
}
void kimphut() {
	TaoVatLieuVer2(RED, NHUA);
	VeHinhLapPhuong(Scale(0.01, 0.17, 0.01), Translate(-0.03, 0.085, 0));

}
void kimgiay() {
	TaoVatLieuVer2(YELLOW, NHUA);
	VeHinhLapPhuong(Scale(0.01, 0.15, 0.01), Translate(-0.03, 0.075, 0));
}
void sogio() {
	TaoVatLieuVer2(BLACK, NHUA);
	bagio();
	muoihaigio();
	saugio();
	chingio();
}

GLfloat kim_giay_quay = 0;
GLfloat kim_gio_quay = 0;
GLfloat kim_phut_quay = 0;
#include <ctime>
void dongho(GLfloat x, GLfloat y, GLfloat z, bool status) {
	model = model * Translate(x, y, z);
	if (status) {
		model = model * RotateY(matXY[1]);
	}
	//Mặt đồng hồ
	VeHinhLapPhuong(Scale(0.05, 0.5, 0.5), mat4());

	time_t now = time(0);
	tm* timeinfo = localtime(&now);
	int hour = timeinfo->tm_hour;
	int min = timeinfo->tm_min;
	int sec = timeinfo->tm_sec;

	sogio();
	mat4 tmp = model;
	kim_giay_quay = 360.0 * sec / 60;
	kim_phut_quay = 360.0 * min / 60 + kim_giay_quay / 60;
	kim_gio_quay = 360.0 * hour / 12 + kim_phut_quay / 12;
	model = model * RotateX(-kim_giay_quay);
	kimgiay();
	model = tmp * RotateX(-kim_phut_quay);
	kimphut();
	model = tmp * RotateX(-kim_gio_quay);
	kimgio();
}
//quạt cây
GLfloat quat_cay_goc_cao = 0, quat_cay_goc_ngang = 0, quat_cay_quay = 0;
GLuint quat_cay_status = 0;
GLfloat quat_cay_status_ngang = 0;

void quat_cay_de()
{
	TaoVatLieuVer2(BLACK, NHUA);
	VeHinhLapPhuong(Scale(0.4, 0.1, 0.4), Translate(0, 0, 0));
}

void quat_cay_truc()
{
	TaoVatLieuVer2(WHITE, NHUA);
	VeHinhLapPhuong(Scale(0.05, 0.6, 0.05), Translate(0.08, 0.3, 0));
}

void quat_cay_moto()
{
	TaoVatLieuVer2(BLACK, NHUA);
	VeHinhLapPhuong(Scale(0.3, 0.2, 0.2), Translate(0.04, 0.65, 0));
}

void quat_cay_truc_moto()
{
	TaoVatLieuVer2(RED, NHUA);
	VeHinhLapPhuong(Scale(0.03, 0.02, 0.02), Translate(-0.12, 0.65, 0));
}

void quat_cay_num_chan()
{
	TaoVatLieuVer2(BLACK, NHUA);
	VeHinhLapPhuong(Scale(0.03, 0.06, 0.06), Translate(-0.15, 0.65, 0));
}

void canh_quat(GLfloat alpha)
{
	TaoVatLieuVer2(WHITE, NHUA);
	VeHinhLapPhuong(Scale(0.01, 0.3, 0.06), RotateX(alpha) * RotateY(10) * Translate(-0.15, 0.18, -0.03));
}

void bo_canh()
{
	quat_cay_truc_moto();
	quat_cay_num_chan();
	model = model * Translate(0, 0.65, 0);
	canh_quat(0);
	canh_quat(90);
	canh_quat(180);
	canh_quat(270);
}
void quat_cay(GLfloat x, GLfloat y, GLfloat z, bool status)
{
	model = model * Translate(x, y, z);
	if (status)
	{
		model = model * RotateY(matXY[1]);
	}
	quat_cay_de();
	quat_cay_truc();
	//xoay ngang
	model = model * Translate(0.04, 0, 0) * RotateY(quat_cay_goc_ngang) * Translate(-0.04, 0, 0);
	//gap
	model = model * Translate(-0.04, 0.65, 0) * RotateZ(quat_cay_goc_cao) * Translate(0.04, -0.65, 0);
	quat_cay_moto();
	//quay
	model = model * Translate(0, 0.65, 0) * RotateX(quat_cay_quay) * Translate(0, -0.65, 0);
	bo_canh();
}

//quạt trần
GLfloat quat_tran_quay = 0;
GLuint quat_tran_status = 0;

void quat_tran_truc()
{
	TaoVatLieuVer2(GRAY, NHUA);
	VeHinhLapPhuong(Scale(0.15, 0.1, 0.15), Translate(0, 0.1, 0));
	VeHinhLapPhuong(Scale(0.05, 0.3, 0.05), Translate(0, 0, 0));
}

void quat_tran_moto()
{
	TaoVatLieuVer2(DARKGREEN, NHUA);
	VeHinhLapPhuong(Scale(0.1, 0.1, 0.1), Translate(0, -0.2, 0));
}
void quat_tran_canh_quat(GLfloat alpha)
{
	TaoVatLieuVer2(GRAY, NHUA);
	VeHinhLapPhuong(Scale(0.9, 0.01, 0.2), RotateY(alpha) * RotateX(5) * Translate(0.5, -0.240, 0));
}
void quat_tran_bo_canh() {
	quat_tran_moto();
	quat_tran_canh_quat(0);
	quat_tran_canh_quat(90);
	quat_tran_canh_quat(180);
	quat_tran_canh_quat(270);
}
void quat_tran(GLfloat x, GLfloat y, GLfloat z, bool status)
{
	model = model * Translate(x, y, z);
	if (status)
	{
		model = model * RotateY(matXY[1]);
	}
	quat_tran_truc();
	model = model * Translate(0, y, 0) * RotateY(-quat_tran_quay) * Translate(0, -y, 0);
	quat_tran_bo_canh();
}

// tivi
GLuint mau_man_hinh = BLACK;

void tivi_vien()
{
	TaoVatLieuVer2(WHITE, NHUA);
	VeHinhLapPhuong(Scale(1.2, 0.03, 0.01), Translate(0, -0.35, 0));
	VeHinhLapPhuong(Scale(1.2, 0.03, 0.01), Translate(0, 0.35, 0));
	VeHinhLapPhuong(Scale(0.03, 0.72, 0.01), Translate(-0.6, 0, 0));
	VeHinhLapPhuong(Scale(0.03, 0.72, 0.01), Translate(0.6, 0, 0));
}

void tivi_man()
{
	TaoVatLieuVer2(mau_man_hinh, NHUA);
	VeHinhLapPhuong(Scale(1.2, 0.7, 0.01), Translate(0, 0, 0));
}

void tivi(GLfloat x, GLfloat y, GLfloat z)
{
	model = model * Translate(x, y, z);
	tivi_man();
	tivi_vien();
}

//bàn
GLfloat chan_ban1_quay = 0, chan_ban2_quay = 0;
void matban() {
	TaoVatLieuVer2(GRAY, NHUA);
	VeHinhLapPhuong(Scale(0.8, 0.01, 1.6), Translate(0, 0, 0));
}
void chanban1() {
	TaoVatLieuVer2(BROWN, NHUA);
	VeHinhLapPhuong(Scale(0.05, 0.4, 0.05), Translate(-0.375, -0.205, 0.775));
	VeHinhLapPhuong(Scale(0.05, 0.4, 0.05), Translate(0.375, -0.205, 0.775));
	VeHinhLapPhuong(Scale(0.75, 0.05, 0.05), Translate(0.0, -0.155, 0.775));
}
void chanban2() {
	TaoVatLieuVer2(BROWN, NHUA);
	VeHinhLapPhuong(Scale(0.05, 0.4, 0.05), Translate(-0.375, -0.205, -0.775));
	VeHinhLapPhuong(Scale(0.05, 0.4, 0.05), Translate(0.375, -0.205, -0.775));
	VeHinhLapPhuong(Scale(0.75, 0.05, 0.05), Translate(0.0, -0.155, -0.775));
}
void ban(GLfloat x, GLfloat y, GLfloat z) {
	model = model * Translate(x, y, z);
	modelchanban2 = model;
	matban();

	model = model * Translate(0, 0, 0.775) * RotateX(chan_ban1_quay) * Translate(0, 0, -0.775);
	chanban1();

	model = modelchanban2;
	model = model * Translate(0, 0, -0.775) * RotateX(chan_ban2_quay) * Translate(0, 0, 0.775);
	chanban2();
}

//sopha
void sopha1(GLfloat x, GLfloat y, GLfloat z) {
	model = model * Translate(x, y, z);
	TaoVatLieuVer2(BROWN, NHUA);
	VeHinhLapPhuong(Scale(0.5, 0.2, 1.6), Translate(0, 0, 0));
	VeHinhLapPhuong(Scale(0.15, 0.2, 1.6), Translate(-0.175, 0.2, 0));
	VeHinhLapPhuong(Scale(0.35, 0.1, 0.1), Translate(0.075, 0.15, 0.775));
	VeHinhLapPhuong(Scale(0.35, 0.1, 0.1), Translate(0.075, 0.15, -0.775));
	//chân
	VeHinhLapPhuong(Scale(0.05, 0.2, 0.05), Translate(-0.225, -0.2, 0.775));
	VeHinhLapPhuong(Scale(0.05, 0.2, 0.05), Translate(0.225, -0.2, 0.775));
	VeHinhLapPhuong(Scale(0.05, 0.2, 0.05), Translate(-0.225, -0.2, -0.775));
	VeHinhLapPhuong(Scale(0.05, 0.2, 0.05), Translate(0.225, -0.2, -0.775));
}

void sopha2(GLfloat x, GLfloat y, GLfloat z) {
	model = model * Translate(x, y, z);
	TaoVatLieuVer2(BROWN, NHUA);
	VeHinhLapPhuong(Scale(0.8, 0.4, 0.4), Translate(0, 0, 0));
}

//điều khiển tivi
void dieukhien(GLfloat x, GLfloat y, GLfloat z) {
	TaoVatLieuVer2(WHITE, NHUA);
	model = model * Translate(x, y, z) * RotateY(90);
	VeHinhLapPhuong(Scale(0.1, 0.01, 0.03), Translate(0, 0, 0));
}

//tranh
void tranh_vien()
{
	TaoVatLieuVer2(BROWN, NHUA);
	VeHinhLapPhuong(Scale(1.5, 0.04, 0.01), Translate(0, -0.42, 0));
	VeHinhLapPhuong(Scale(1.5, 0.04, 0.01), Translate(0, 0.42, 0));
	VeHinhLapPhuong(Scale(0.04, 0.8, 0.01), Translate(-0.73, 0, 0));
	VeHinhLapPhuong(Scale(0.04, 0.8, 0.01), Translate(0.73, 0, 0));
}

void tranh_man()
{
	TaoVatLieuVer2(GRAY, NHUA);
	VeHinhLapPhuong(Scale(1.42, 0.8, 0.01), Translate(0, 0, 0));
}

void tranh(GLfloat x, GLfloat y, GLfloat z)
{
	model = model * Translate(x, y, z) * RotateY(90);
	tranh_man();
	tranh_vien();
}

//đèn
void den(GLfloat x, GLfloat y, GLfloat z) {
	TaoVatLieuVer2(WHITE, NHUA);
	model = model * Translate(x, y, z) * RotateY(90);
	VeHinhLapPhuong(Scale(1.0, 0.05, 0.05), Translate(0, 0, 0));
}


void loop() {
	//viTriAnhSang = eye;
	//quạt cây
	quat_cay_quay -= quat_cay_status * 1;
	if (quat_cay_quay >= 360) {
		quat_cay_quay -= 360;
	}
	if (quat_cay_goc_ngang >= 70) {
		quat_cay_status_ngang = -0.05;
	}
	if (quat_cay_goc_ngang <= -70) {
		quat_cay_status_ngang = 0.05;
	}
	quat_cay_goc_ngang += quat_cay_status_ngang;
	//quạt trần
	quat_tran_quay += quat_tran_status * 0.3;
	if (quat_tran_quay >= 360) {
		quat_tran_quay -= 360;
	}
	//cửa cuốn
	//cua_cuon_status = 1;
	if (cua_cuon_status == 1) {
		cua_cuon_do_dai -= 0.01;
	}
	if (cua_cuon_status == 0) {
		cua_cuon_do_dai += 0.01;
	}
	if (cua_cuon_do_dai < 0) {
		cua_cuon_do_dai = 0;
		cua_cuon_status = 2;
	}
	if (cua_cuon_do_dai > 4) {
		cua_cuon_do_dai = 4;
		cua_cuon_status = 2;
	}
	glutPostRedisplay();
};

bool ban_trang_thai = false;
bool quatcay_trang_thai = false;
bool sopha1_trang_thai = false;
bool sopha2_trang_thai = false;
bool dieukhien_trang_thai = false;

void display(void)
{
	loop();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Ngoại cảnh
	ngoai_canh();

	//Định hướng mắt nhìn vào phòng khách
	at = eye + normalize(mat_dinhhuong);
	eye.w = at.w = 1;

	//cửa cuốn
	model = mat4();
	cua_cuon();

	//đồng hồ
	model = mat4();
	dongho(4.925, 3.4, 4.0, 0);

	//quạt trần
	model = mat4();
	quat_tran(2.5, 3.8, 3.0, 0);

	//quạt cây
	model = mat4();
	//quat_cay(2.1, 0.1, 4.0, 0);
	if (quatcay_trang_thai == false) {
		quat_cay(2.1, 0.1, 4.0, 0);
	}
	else {
		vec4 tmp5 = eye + mat_dinhhuong;
		tmp5.y = 0.5;
		quat_cay(tmp5.x, tmp5.y, tmp5.z + 0.4, 0);
	}

	//tivi
	TaoVatLieuVer2(GRAY, BAC);
	model = mat4();
	tivi(1.2, 1.8, 6.0);

	//bàn
	model = mat4();
	if (ban_trang_thai == false) {
		model = mat4();
		if (chan_ban1_quay != 0 || chan_ban2_quay != 0) {
			model = model * RotateZ(180);
			ban(-1.2, -0.205, 3.2);
		}
		else {
			ban(1.2, 0.405, 3.2);
		}
	}
	else {
		vec4 tmp = eye + mat_dinhhuong;
		tmp.y = 1.2;
		ban(tmp.x, tmp.y, tmp.z + 0.4);
	}

	//sopha
		//sopha1
	model = mat4();
	//sopha1(0.4, 0.3, 3.2);
	if (sopha1_trang_thai == false) {
		sopha1(0.4, 0.3, 3.2);
	}
	else {
		dieukhien_trang_thai = true;
		vec4 tmp2 = eye + mat_dinhhuong;
		tmp2.y = 1.2;
		sopha1(tmp2.x, tmp2.y, tmp2.z + 0.4);
	}
	//sopha2
	model = mat4();
	//sopha2(1.2, 0.205, 4.4);
	if (sopha2_trang_thai == false) {
		sopha2(1.2, 0.205, 4.4);
	}
	else {
		vec4 tmp4 = eye + mat_dinhhuong;
		tmp4.y = 1.2;
		sopha2(tmp4.x, tmp4.y, tmp4.z + 0.4);
	}

	//điều khiển
	model = mat4();
	/*dieukhien(0.49, 0.45, 3.5);*/
	if (dieukhien_trang_thai == false) {
		dieukhien(0.49, 0.45, 3.5);
	}
	else {
		vec4 tmp3 = eye + mat_dinhhuong;
		tmp3.y = 1.3;
		dieukhien(tmp3.x, tmp3.y, tmp3.z + 0.4);
		//mau_man_hinh = DARKGREEN;
	}
	//tranh
	model = mat4();
	tranh(0.01, 2.0, 3.2);

	//đèn
	model = mat4();
	den(0.05, 3.5, 3.2);

	//view
	eye.w = 1;
	at.w = 1;
	view = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);

	projection = Frustum(l, r, b, t, zN, zF);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	// keyboard handler

	switch (key) {
	case 033:			// 033 is Escape key octal value
		exit(1);		// quit program
		break;
	case '[':
		cua_cuon_status = 1;
		break;
	case ']':
		cua_cuon_status = 0;
		break;
	case 'w':
		eye = eye + RotateY(matXY[1]) * vec4(0.0, 0.0, 0.2, 1);
		break;
	case 's':
		eye = eye - RotateY(matXY[1]) * vec4(0.0, 0.0, 0.2, 1);
		break;
	case 'd':
		eye = eye + RotateY(matXY[1]) * vec4(0.2, 0.0, 0.0, 1);
		break;
	case 'a':
		eye = eye - RotateY(matXY[1]) * vec4(0.2, 0.0, 0.0, 1);
		break;
	case 'j':
		matXY[1] -= 3;
		if (matXY[1] >= 360) matXY[1] -= 360;
		if (matXY[1] < 0) matXY[1] += 360;
		break;
	case 'l':
		matXY[1] += 3;
		if (matXY[1] >= 360) matXY[1] -= 360;
		if (matXY[1] < 0) matXY[1] += 360;
		break;
	case 'i':
		matXY[0] -= 3;
		if (matXY[0] < -70) {
			matXY[0] = -70;
		}
		break;
	case 'k':
		matXY[0] += 3;
		if (matXY[0] > 70) {
			matXY[0] = 70;
		}
		break;
	case 't':
		chan_ban1_quay += 5.0;
		if (chan_ban1_quay > 80) chan_ban1_quay = 80;
		break;
	case 'T':
		chan_ban1_quay -= 5.0;
		if (chan_ban1_quay < 0) chan_ban1_quay = 0;
		break;
	case 'y':
		chan_ban2_quay -= 5.0;
		if (chan_ban2_quay < -80) chan_ban2_quay = -80;
		break;
	case 'Y':
		chan_ban2_quay += 5.0;
		if (chan_ban2_quay > 0) chan_ban2_quay = 0;
		break;
	case 'x':
		quat_cay_status += 1;
		quat_cay_status %= 4;
		break;
	case 'X':
		if (quat_cay_status_ngang == 0)
			quat_cay_status_ngang = 0.05;
		else quat_cay_status_ngang = 0;
		break;
	case 'z':
		quat_cay_goc_cao += 1;
		if (quat_cay_goc_cao > 20) {
			quat_cay_goc_cao = 20;
		}
		break;
	case 'Z':
		quat_cay_goc_cao -= 1;
		if (quat_cay_goc_cao < -20) {
			quat_cay_goc_cao = -20;
		}
		break;
	case 'b':
		quat_tran_status += 1;
		quat_tran_status %= 6;
		break;
	case 'o':
		if (mau_man_hinh == BLACK) {
			mau_man_hinh = DARKGREEN;
		}
		else {
			mau_man_hinh = BLACK;
		}
		break;
	case 'q':
		ti_le_anh_sang -= 0.01;
		if (ti_le_anh_sang < 0)
		{
			ti_le_anh_sang = 0;
		}
		break;
	case 'e':
		ti_le_anh_sang += 0.01;
		if (ti_le_anh_sang > 1)
		{
			ti_le_anh_sang = 1;
		}
		break;
	case '1':
		ban_trang_thai = !ban_trang_thai;
		break;
	case '2':
		sopha1_trang_thai = !sopha1_trang_thai;
		break;
	case '3':
		dieukhien_trang_thai = !dieukhien_trang_thai;
		break;
	case '4':
		sopha2_trang_thai = !sopha2_trang_thai;
		break;
	case '5':
		quatcay_trang_thai = !quatcay_trang_thai;
		break;
	}
	mat_dinhhuong = RotateY(matXY[1]) * RotateX(matXY[0]) * vec4(0, 0, 1, 1);
	mat_dinhhuong.w = 1;
	//cout << "eye " << eye << endl << "at " << at << endl;
	glutPostRedisplay();

}


int main(int argc, char** argv)
{
	setup_color();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(720, 720);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Living room");


	glewInit();

	generateGeometry();
	initGPUBuffers();
	shaderSetup();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	//glutReshapeFunc(reshape);

	glutMainLoop();
	return 0;
}
