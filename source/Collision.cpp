#include "Collision.h"     
#include "Player.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
#define ports Player::portals
bool handlePortalSphereCollision(Sphere& s) {

    for (int i = 0; i < 2; i++) {
        if (!s.available) return false;
        if (checkForPortalSphereCollision(s,*ports[i])) {
            s.modelMatrix = ports[1 - i]->modelMatrix * rotate(mat4(1.0f), radians(180.0f), vec3(0, 1.0f, 0))*inverse(ports[i]->modelMatrix);
            //In order to keep the new model matrix we need to use the position vector
            s.v = vec3(s.modelMatrix * vec4(s.v,0.0f));
            s.x = vec3(s.modelMatrix*vec4(s.x,1.0f))+ports[1-i]->normal*1.9f*s.r;
            
            s.P = s.v * s.m;
            s.update();
            return true;
            //s.x = ports[1 - i]->position + ports[1-i]->normal*(s.r)+0.2f;
            //printf("%f %f %f\n", s.x.x, s.x.y, s.x.z);
            
        }
    }
    return false;
}
void handleBasketSphereCollision(Basket& basket, Sphere& s) {
    vec3 n;
    if (checkForBasketSphereCollision(basket, s, n)) {
        printf("stefani\n");
        s.v = -0.8f*s.v;
        s.x += normalize(s.v)*0.2f;
        s.P = s.v * s.m;
    }
}
void handleBoxSphereCollision(Box& box, Sphere& sphere) {
    vec3 n;
    if (checkForBoxSphereCollision(sphere.x, sphere.r, box.maxx,box.maxy,box.maxz, n)) {

        //printf("wall\n");
        sphere.v = (sphere.v - n * glm::dot(sphere.v, n) * 1.7f);
        sphere.P = sphere.m * sphere.v;
    }
}
void handleSphereSphereCollision(Sphere& sphere1, Sphere& sphere2) {
    if (checkForSphereSphereCollision(sphere1.x, sphere2.x, sphere1.v, sphere2.v, sphere1.r, sphere2.r)) {
        sphere1.v = sphere1.v - 2 * sphere2.m / (sphere1.m + sphere2.m) * dot(sphere1.v - sphere2.v, sphere1.x - sphere2.x)
            * (sphere1.x - sphere2.x) / distance(sphere1.x, sphere2.x) / distance(sphere1.x, sphere2.x);
        
        sphere2.v = sphere2.v - 2 * sphere1.m / (sphere2.m + sphere1.m) * dot(sphere2.v - sphere1.v, sphere2.x - sphere1.x)
            * (sphere2.x - sphere1.x) / distance(sphere2.x, sphere1.x) / distance(sphere2.x, sphere1.x);


    }
}
void handlePortalPlayerCollision(Player& pl,float t, float dt) {
    for (int i = 0; i < 2; i++) {
        if (checkForPortalPlayerCollision(*ports[i],pl)) {
            pl.x = ports[1 - i]->position+ports[1-i]->normal;
            /*
            vec3 v1 = normalize(vec3(ports[i]->normal.x, 0, ports[i]->normal.z));
            vec3 v2 = normalize(vec3(ports[1-i]->normal.x, 0, ports[1-i]->normal.z));
            if (length(v1) > 0.5f && length(v2) > 0.5f ) {

                float df = acos(dot(v1,v2));
                printf("%f\n",df);
                pl.cam->horizontalAngle -= df;
            }
            */
            //recalculate horizontal angle
            pl.cam->horizontalAngle = acos(dot(vec3(0,0,1.0f),ports[1-i]->normal));
            pl.cam->direction = vec3(
                cos(pl.cam->verticalAngle) * sin(pl.cam->horizontalAngle),
                sin(pl.cam->verticalAngle),
                cos(pl.cam->verticalAngle) * cos(pl.cam->horizontalAngle)
            );
            //correction bc acos ranges (-pi/2,pi/2)
            if (dot(pl.x,pl.cam->direction)>0) {
                pl.cam->horizontalAngle = -pl.cam->horizontalAngle;
            }
            //printf("%f %f %f\n", pl.cam->direction.x, pl.cam->direction.y, pl.cam->direction.z);

            //readjust height
            pl.jumped = true;
            pl.forcing = [&](float t2, const vector<float>& y)->vector<float> {
                vector<float> f(6, 0.0f);
                f[1] = -pl.m * 9.81;
                f[0] = -0.3f * pl.v.x;
                f[2] = -0.3f * pl.v.z;
                return f;
            };
            

        }
    }
}




bool checkForBoxSphereCollision(vec3 &pos, const float& r,
                                const float& maxx, const float& maxy, const float& maxz, vec3& n) {
    if (pos.x - r <= -maxx) {
        //correction
        float dis = -maxx-(pos.x - r);
        pos = pos + vec3(dis, 0, 0);

        n = vec3(-1, 0, 0);
    } else if (pos.x + r >= maxx) {
        //correction
        float dis = maxx - (pos.x + r);
        pos = pos + vec3(dis, 0, 0);

        n = vec3(1, 0, 0);
    } else if (pos.y - r <= 0) {
        //correction
        float dis = -(pos.y - r);
        pos = pos + vec3(0, dis, 0);

        n = vec3(0, -1, 0);
    } else if (pos.y + r >= maxy ) {
        //correction
        float dis = maxy - (pos.y + r);
        pos = pos + vec3(0, dis, 0);

        n = vec3(0, 1, 0);
    } else if (pos.z - r <= -maxz) {
        //correction
        float dis = -maxz-(pos.z - r);
        pos = pos + vec3(0, 0, dis);

        n = vec3(0, 0, -1);
    } else if (pos.z + r >= maxz) {
        //correction
        float dis = maxz - (pos.z + r);
        pos = pos + vec3(0, 0, dis);

        n = vec3(0, 0, 1);
    } else {
        return false;
    }
    return true;
}
bool checkForSphereSphereCollision(glm::vec3& pos1, glm::vec3& pos2,glm::vec3& v1, glm::vec3& v2,
    const float& r1, const float& r2) {
    float d = distance(pos1, pos2);
    if (d < r1 + r2) {
        //correction or else they stay stuck
        vec3 n=normalize(pos1 - pos2);
        pos1 += n * (r1 + r2 - d) * 0.51f;
        pos2 -= n * (r1 + r2 - d) * 0.51f;
        
        return true;
    }
    return false;

}

bool checkForPortalSphereCollision(Sphere& s, Portal& p) {
    vec3 r = s.x - p.position;
    // sxetikh thesh
    vec3 plane_proj = r - p.normal * dot(r,p.normal);
    //printf("%f %f %f\n", plane_proj.x,plane_proj.y,plane_proj.z );
    float condition1 = plane_proj.z; //height check
    float condition2 = plane_proj.x; //width check
    if (abs(dot(p.normal,vec3(0,1.0f,0)))>0.98f) {
        condition1 = plane_proj.z;
        condition2 = plane_proj.x;
    }else if (abs(dot(p.normal,vec3(1.0f,0,0)))>0.98f) {
        condition1 = plane_proj.y;
        condition2 = plane_proj.z;
    }else if (abs(dot(p.normal,vec3(0,0,1.0f)))>0.98f) {
        condition1 = plane_proj.y;
        condition2 = plane_proj.x;
    }else printf("Error\n");
    //printf("%f %f %f\n", s.v.x,s.v.y,s.v.z);
    //printf("%f %f %f\n", p.normal.x,p.normal.y,p.normal.z);
    if (abs(dot(r, p.normal))-s.r <= 0.1f 
        && abs(condition1)<p.height/2 -s.r 
        && abs(condition2) < p.width/2 - s.r
        && dot(s.v,p.normal) < 0) {
        //printf("collision\n");
        return true;
    }
    return false;
}


bool checkForBasketSphereCollision(Basket& b, Sphere& s, vec3& n) {
    vec3 r =s.x-b.position;
    vec3 plane_proj = r - dot(r, vec3(0, 1.0f, 0));
    //printf("%f %f %f\n",plane_proj.x,plane_proj.y,plane_proj.z);
    if (length(plane_proj) - s.r < b.r && abs(s.x.y - b.position.y) < s.r) { 
        printf("stef\n");
        return true; 
    }
    return false;
}

bool checkForPortalPlayerCollision(Portal& p,Player& player) {
    vec3 r = player.cam->position - p.position;
    // sxetikh thesh
    vec3 plane_proj = r - p.normal * dot(r, p.normal);
    //printf("%f %f %f\n", plane_proj.x,plane_proj.y,plane_proj.z );
    float condition1 = plane_proj.z; //height check
    float condition2 = plane_proj.x; //width check
    if (abs(dot(p.normal, vec3(0, 1.0f, 0))) > 0.98f) {
        condition1 = plane_proj.z;
        condition2 = plane_proj.x;
    }
    else if (abs(dot(p.normal, vec3(1.0f, 0, 0))) > 0.98f) {
        condition1 = plane_proj.y;
        condition2 = plane_proj.z;
    }
    else if (abs(dot(p.normal, vec3(0, 0, 1.0f))) > 0.98f) {
        condition1 = plane_proj.y;
        condition2 = plane_proj.x;
    }
    else printf("Error\n");
    //printf("%f %f %f\n", s.v.x,s.v.y,s.v.z);
    //printf("%f %f %f\n", p.normal.x,p.normal.y,p.normal.z);
    if (dot(p.normal,vec3(0,-1,0))>0.98) return false;

    if (abs(dot(r, p.normal)) <= 0.2f
        && abs(condition1) < p.height / 2
        && abs(condition2) < p.width / 2) {
        //printf("PLAYER TP\n");
        return true;
    }
    if (length(player.position - p.position - vec3(0, 1, 0)) < 0.4f) { 
        //player.disable = true;
        return true; 
    }
    return false;
}