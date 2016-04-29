/*
 
 AppSkeleton Autotools project to start off with for MacOS X.
 
 jared@lostsidedead.com
 
 This source is in the public domain.
 
 Special Thanks to John Tsiombikas <nuclear@member.fsf.org> for writing such a good example I could learn from.
 This is just a test program trying to get all the things in place to make something cool.
 A lot of this is adapted from the example and some of it straight out of it.
 I am still learning how to code for Oculus 2.
 
 */



#include"glew.h"
#include"SDL.h"
#include<iostream>
#include <OVR_CAPI_0_5_0.h>
#include<OVR_CAPI_GL.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string>



ovrHmd hmd;
ovrSizei eyeres[2];
ovrEyeRenderDesc eye_rdesc[2];
ovrGLTexture fb_ovr_tex[2];
static unsigned int chess_tex;
union ovrGLConfig glcfg;
unsigned int distort_caps;
unsigned int hmd_caps;
int fb_width, fb_height;
int fb_tex_width, fb_tex_height;
int win_width, win_height;

SDL_Surface *texture1;
GLuint texture_1_id;
GLuint fbo = 0, fb_tex, fb_depth;
void update_rtarg(int width, int height);
unsigned int next_pow2(unsigned int x);
void quat_to_matrix(const float *quat, float *mat);
void draw_scene();
void draw_box();



void draw() {
    draw_scene();
}

void render() {
    int i;
    ovrMatrix4f proj;
    ovrPosef pose[2];
    float rot_mat[16];
    static float val = 0.0f;
    
    val += 0.01f;
    if(val>1.0f) val = 0;
    
    
    ovrHmd_BeginFrame(hmd, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(0.3f, 0.3f, 0.3f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for(i = 0; i<2; i++) {
        ovrEyeType eye= hmd->EyeRenderOrder[i];
        glViewport(eye == ovrEye_Left ? 0 : fb_width / 2, 0, fb_width / 2, fb_height);
        proj = ovrMatrix4f_Projection(hmd->DefaultEyeFov[eye], 0.5, 500.0, 1);
        glMatrixMode(GL_PROJECTION);
        glLoadTransposeMatrixf(proj.M[0]);
        pose[eye] = ovrHmd_GetHmdPosePerEye(hmd, eye);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(eye_rdesc[eye].HmdToEyeViewOffset.x,eye_rdesc[eye].HmdToEyeViewOffset.y,eye_rdesc[eye].HmdToEyeViewOffset.z);
        quat_to_matrix(&pose[eye].Orientation.x, rot_mat);
        glMultMatrixf(rot_mat);
        glTranslatef(-pose[eye].Position.x, -pose[eye].Position.y, -pose[eye].Position.z);
        glTranslatef(0, -ovrHmd_GetFloat(hmd, OVR_KEY_EYE_HEIGHT, 1.65), 0);
        draw();
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ovrHmd_EndFrame(hmd, pose, &fb_ovr_tex[0].Texture);
    
    glUseProgram(0);
    
    
}

void init_device(SDL_Window *window) {
    int i = 0;
    ovr_Initialize(0);
    
    if(!(hmd = ovrHmd_Create(0))) {
        std::cerr << "Error could not create device..\n";
        if(!(hmd = ovrHmd_CreateDebug(ovrHmd_DK2))) {
            std::cerr << "failed to create virtual debug device\n";
            exit(0);
        }
    }
    ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation |  ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position, 0);
    std::cout << "initialized HMD: " << hmd->Manufacturer << " - " << hmd->ProductName << "\n";
    eyeres[0] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left, hmd->DefaultEyeFov[0], 1.0);
    eyeres[1] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right, hmd->DefaultEyeFov[1], 1.0);
    fb_width = eyeres[0].w + eyeres[1].w;
    fb_height = eyeres[0].h > eyeres[1].h ? eyeres[0].h : eyeres[1].h;
    win_width = hmd->Resolution.w;
    win_height = hmd->Resolution.h;
    glClearColor(1.0f, 1.0f, 1.0f, 1.0);
    update_rtarg(fb_width, fb_height);
    for(i=0; i<2; i++) {
        fb_ovr_tex[i].OGL.Header.API = ovrRenderAPI_OpenGL;
        fb_ovr_tex[i].OGL.Header.TextureSize.w = fb_tex_width;
        fb_ovr_tex[i].OGL.Header.TextureSize.h = fb_tex_height;
        fb_ovr_tex[i].OGL.Header.RenderViewport.Pos.x = i == 0 ? 0 : fb_width / 2.0;
        fb_ovr_tex[i].OGL.Header.RenderViewport.Pos.y = 0;
        fb_ovr_tex[i].OGL.Header.RenderViewport.Size.w = fb_width / 2.0;
        fb_ovr_tex[i].OGL.Header.RenderViewport.Size.h = fb_height;
        fb_ovr_tex[i].OGL.TexId = fb_tex;
    }
    SDL_SetWindowSize(window, hmd->Resolution.w, hmd->Resolution.h);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    memset(&glcfg, 0, sizeof glcfg);
    glcfg.OGL.Header.API = ovrRenderAPI_OpenGL;
    glcfg.OGL.Header.BackBufferSize.w = win_width;
    glcfg.OGL.Header.BackBufferSize.h = win_height;
    glcfg.OGL.Header.Multisample = 1;
    if(hmd->HmdCaps & ovrHmdCap_ExtendDesktop) {
        std::cout << "running in \"extended desktop\" mode\n";
    } else {
        std::cout << "running in \"direct-hmd\" mode\n";
        hmd_caps = ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction;
        ovrHmd_SetEnabledCaps(hmd, hmd_caps);
    }
    distort_caps = ovrDistortionCap_TimeWarp | ovrDistortionCap_Overdrive;
    if(!ovrHmd_ConfigureRendering(hmd, &glcfg.Config, distort_caps, hmd->DefaultEyeFov, eye_rdesc)) {
        std::cerr << "failed to configure distortion renderer\n";
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    
    glClearColor(1.0, 1.0, 1.0, 1);
}


void update_rtarg(int width, int height)
{
    if(!fbo) {
        glGenFramebuffers(1, &fbo);
        glGenTextures(1, &fb_tex);
        glGenRenderbuffers(1, &fb_depth);
        
        glBindTexture(GL_TEXTURE_2D, fb_tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    fb_tex_width = next_pow2(width);
    fb_tex_height = next_pow2(height);
    glBindTexture(GL_TEXTURE_2D, fb_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fb_tex_width, fb_tex_height, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_tex, 0);
    
    glBindRenderbuffer(GL_RENDERBUFFER, fb_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fb_tex_width, fb_tex_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fb_depth);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr <<  "incomplete framebuffer!\n";
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    std::cout << "created render target: " << width << "x" << height << " (texture size: " << fb_tex_width << "x" << fb_tex_height << ")\n";
}



unsigned int next_pow2(unsigned int x)
{
    x -= 1;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}

void quat_to_matrix(const float *quat, float *mat)
{
    mat[0] = 1.0 - 2.0 * quat[1] * quat[1] - 2.0 * quat[2] * quat[2];
    mat[4] = 2.0 * quat[0] * quat[1] + 2.0 * quat[3] * quat[2];
    mat[8] = 2.0 * quat[2] * quat[0] - 2.0 * quat[3] * quat[1];
    mat[12] = 0.0f;
    mat[1] = 2.0 * quat[0] * quat[1] - 2.0 * quat[3] * quat[2];
    mat[5] = 1.0 - 2.0 * quat[0]*quat[0] - 2.0 * quat[2]*quat[2];
    mat[9] = 2.0 * quat[1] * quat[2] + 2.0 * quat[3] * quat[0];
    mat[13] = 0.0f;
    mat[2] = 2.0 * quat[2] * quat[0] + 2.0 * quat[3] * quat[1];
    mat[6] = 2.0 * quat[1] * quat[2] - 2.0 * quat[3] * quat[0];
    mat[10] = 1.0 - 2.0 * quat[0]*quat[0] - 2.0 * quat[1]*quat[1];
    mat[14] = 0.0f;
    mat[3] = mat[7] = mat[11] = 0.0f;
    mat[15] = 1.0f;
}

void draw_box(float xsz, float ysz, float zsz, float norm_sign)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glScalef(xsz * 0.5, ysz * 0.5, zsz * 0.5);
    
    if(norm_sign < 0.0) {
        glFrontFace(GL_CW);
    }
    
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1 * norm_sign);
    glTexCoord2f(0, 0); glVertex3f(-1, -1, 1);
    glTexCoord2f(1, 0); glVertex3f(1, -1, 1);
    glTexCoord2f(1, 1); glVertex3f(1, 1, 1);
    glTexCoord2f(0, 1); glVertex3f(-1, 1, 1);
    glNormal3f(1 * norm_sign, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(1, -1, 1);
    glTexCoord2f(1, 0); glVertex3f(1, -1, -1);
    glTexCoord2f(1, 1); glVertex3f(1, 1, -1);
    glTexCoord2f(0, 1); glVertex3f(1, 1, 1);
    glNormal3f(0, 0, -1 * norm_sign);
    glTexCoord2f(0, 0); glVertex3f(1, -1, -1);
    glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
    glTexCoord2f(1, 1); glVertex3f(-1, 1, -1);
    glTexCoord2f(0, 1); glVertex3f(1, 1, -1);
    glNormal3f(-1 * norm_sign, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(-1, -1, -1);
    glTexCoord2f(1, 0); glVertex3f(-1, -1, 1);
    glTexCoord2f(1, 1); glVertex3f(-1, 1, 1);
    glTexCoord2f(0, 1); glVertex3f(-1, 1, -1);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 1 * norm_sign, 0);
    glTexCoord2f(0.5, 0.5); glVertex3f(0, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(-1, 1, 1);
    glTexCoord2f(1, 0); glVertex3f(1, 1, 1);
    glTexCoord2f(1, 1); glVertex3f(1, 1, -1);
    glTexCoord2f(0, 1); glVertex3f(-1, 1, -1);
    glTexCoord2f(0, 0); glVertex3f(-1, 1, 1);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, -1 * norm_sign, 0);
    glTexCoord2f(0.5, 0.5); glVertex3f(0, -1, 0);
    glTexCoord2f(0, 0); glVertex3f(-1, -1, -1);
    glTexCoord2f(1, 0); glVertex3f(1, -1, -1);
    glTexCoord2f(1, 1); glVertex3f(1, -1, 1);
    glTexCoord2f(0, 1); glVertex3f(-1, -1, 1);
    glTexCoord2f(0, 0); glVertex3f(-1, -1, -1);
    glEnd();
    
    glFrontFace(GL_CCW);
    glPopMatrix();
}


void draw_scene(void)
{
    int i;
    float grey[] = {0.8, 0.8, 0.8, 1};
    float col[] = {0, 0, 0, 1};
    float lpos[][4] = {
        {-8, 2, 10, 1},
        {0, 15, 0, 1}
    };
    float lcol[][4] = {
        {0.8, 0.8, 0.8, 1},
        {0.4, 0.3, 0.3, 1}
    };
    
    for(i=0; i<2; i++) {
        glLightfv(GL_LIGHT0 + i, GL_POSITION, lpos[i]);
        glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, lcol[i]);
    }
    
    glMatrixMode(GL_MODELVIEW);
    
    glPushMatrix();
    glTranslatef(0, 10, 0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
    glBindTexture(GL_TEXTURE_2D, texture_1_id);
    glEnable(GL_TEXTURE_2D);
    draw_box(30, 20, 30, -1.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    for(i=0; i<4; i++) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
        glPushMatrix();
        glTranslatef(i & 1 ? 5 : -5, 1, i & 2 ? -5 : 5);
        draw_box(0.5, 2, 0.5, 1.0);
        glPopMatrix();
        
        col[0] = i & 1 ? 1.0 : 0.3;
        col[1] = i == 0 ? 1.0 : 0.3;
        col[2] = i & 2 ? 1.0 : 0.3;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, col);
        
        glPushMatrix();
        if(i & 1) {
            glTranslatef(0, 0.25, i & 2 ? 2 : -2);
        } else {
            glTranslatef(i & 2 ? 2 : -2, 0.25, 0);
        }
        draw_box(0.5, 0.5, 0.5, 1.0);
        glPopMatrix();
    }
    
    col[0] = 1;
    col[1] = 1;
    col[2] = 0.4;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, col);
    draw_box(0.05, 1.2, 6, 1.0);
    draw_box(6, 1.2, 0.05, 1.0);
    
    
}
void keyEvent(SDL_Event &e, int key, int state) {
    switch(key) {
        case SDLK_r:
            ovrHmd_RecenterPose(hmd);
            break;
    }
}

void clean() {
    if(hmd) {
        ovrHmd_Destroy(hmd);
    }
    ovr_Shutdown();
}


int main(int argc, char **argv) {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        std::cerr << "Error initailizing..\n";
        return -1;
    }
    SDL_Event e;
    win_width = 1280;
    win_height = 720;
    SDL_Window *window = SDL_CreateWindow("Oculus", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_width, win_height, SDL_WINDOW_OPENGL);
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    glewExperimental = GL_TRUE;
    glewInit();
    std::cout << glGetString(GL_VERSION) << "\n";
    init_device(window);
    bool active = true;
    while(active) {
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT:
                    active = false;
                    break;
                case SDL_KEYDOWN:
                    ovrHSWDisplayState hsw;
                    ovrHmd_GetHSWDisplayState(hmd, &hsw);
                    if(hsw.Displayed) {
                        ovrHmd_DismissHSWDisplay(hmd);
                    }
                    
                    if(e.key.keysym.sym == SDLK_ESCAPE)
                        active = false;
                    else {
                        keyEvent(e, e.key.keysym.sym, 0);
                    }
                    break;
            }
        }
        render();
    }
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    clean();
    SDL_Quit();
    return 0;
}
