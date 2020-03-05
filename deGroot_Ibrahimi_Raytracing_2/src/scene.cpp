#include "scene.h"

#include "hit.h"
#include "image.h"
#include "material.h"
#include "ray.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>

using namespace std;

pair<ObjectPtr, Hit> Scene::castRay(Ray const& ray) const {
    // Find hit object and distance
    Hit min_hit(numeric_limits<double>::infinity(), Vector());
    ObjectPtr obj = nullptr;
    for (unsigned idx = 0; idx != objects.size(); ++idx) {
        Hit hit(objects[idx]->intersect(ray));
        if (hit.t < min_hit.t) {
            min_hit = hit;
            obj = objects[idx];
        }
    }

    return pair<ObjectPtr, Hit>(obj, min_hit);
}

Color Scene::trace(Ray const& ray, unsigned depth) {
    pair<ObjectPtr, Hit> mainhit = castRay(ray);
    ObjectPtr obj = mainhit.first;
    Hit min_hit = mainhit.second;

    // No hit? Return background color.
    if (!obj)
        return Color(0.0, 0.0, 0.0);

    Material const& material = obj->material;
    Point hit = ray.at(min_hit.t);
    Vector V = -ray.D;

    // Pre-condition: For closed objects, N points outwards.
    Vector N = min_hit.N;

    // The shading normal always points in the direction of the view,
    // as required by the Phong illumination model.
    Vector shadingN;
    if (N.dot(V) >= 0.0)
        shadingN = N;
    else
        shadingN = -N;

	Color matColor;
	if (material.hasTexture) {
		Vector UV = obj->toUV(hit);
		matColor = material.texture.colorAt(UV.x, UV.y);
	} else {
		matColor = material.color;
	}

    // Add ambient once, regardless of the number of lights.
    Color color = material.ka * matColor;

    // Add diffuse and specular components.
	for (auto const& light : lights) {
		Vector L = (light->position - hit).normalized();

		// Calculate illumination multiplier. For now, 0 when light source is blocked
		double illumination = 1.0;
		if (renderShadows) {
			auto lightIntersect = Scene::castRay({hit + epsilon * shadingN, L});
			if (lightIntersect.first &&
				lightIntersect.second.t < (light->position - hit).length())
				illumination = 0.0;
		}

		// Add diffuse.
		double diffuse = std::max(shadingN.dot(L), 0.0);
		color += illumination * diffuse * material.kd * light->color * matColor;

		// Add specular.
		Vector reflectDir = reflect(-L, shadingN);
		double specAngle = std::max(reflectDir.dot(V), 0.0);
		double specular = std::pow(specAngle, material.n);

		color += illumination * specular * material.ks * light->color;
	}

    if (depth > 0 and material.isTransparent) {
        // The object is transparent, and thus refracts and reflects light.
        // Use Schlick's approximation to determine the ratio between the two.
        double ni, nt;
        if (N.dot(V) >= 0.0) { // ray from outside to inside
            ni = 1.0;
            nt = material.nt;
        } else { // ray from inside to outside
            ni = material.nt;
            nt = 1.0;
        }

        double kr0 = pow((ni - nt) / (ni + nt), 2);
        double kr = kr0 + (1 - kr0) * pow((1 - shadingN.dot(V)), 5);
        double kt = 1 - kr;

        Vector R = (2 * shadingN.dot(V) * shadingN - V).normalized();
        Color transRefl = trace({hit + epsilon * shadingN, R}, depth - 1);
        if (kr > 0.0) color += kr * transRefl;

        Vector D = ray.D;
        double t = 1 - (ni * ni * (1 - pow(D.dot(shadingN), 2))) / (nt * nt);
        if (t > 0.0) {
			Vector T = ((ni * (D - D.dot(shadingN) * shadingN)) / nt - shadingN * sqrt(t)).normalized();
			Color transmitted = trace({hit - epsilon * shadingN, T}, depth - 1);

			if (kt > 0.0) color += kt * transmitted;
		}

    } else if (depth > 0 and material.ks > 0.0) {
        // The object is not transparent, but opaque.
        Vector R = (2 * shadingN.dot(V) * shadingN - V).normalized();
        Color opaqRefl = trace({hit + epsilon * shadingN, R}, depth - 1);
        color += opaqRefl * material.ks;
    }

    return color;
}

void Scene::render(Image& img) {
    unsigned w = img.width();
    unsigned h = img.height();
    double d = 1.0 / (supersamplingFactor + 1.0);

    for (unsigned y = 0; y < h; ++y) {
        for (unsigned x = 0; x < w; ++x) {
			Color col = Color(0.0, 0.0, 0.0);
			for (unsigned dy = 1; dy <= supersamplingFactor; ++dy) {
				for (unsigned dx = 1; dx <= supersamplingFactor; ++dx) {
					Point pixel(x + dx * d, h - 1 - y + dy * d, 0);
					Ray ray(eye, (pixel - eye).normalized());
					col += trace(ray, recursionDepth);
				}
			}
			col /= supersamplingFactor * supersamplingFactor;
			col.clamp();
			img(x, y) = col;
        }
	}
}

// --- Misc functions ----------------------------------------------------------

// Defaults
Scene::Scene()
        :
        objects(),
        lights(),
        eye(),
        renderShadows(false),
        recursionDepth(0),
        supersamplingFactor(1) {}

void Scene::addObject(ObjectPtr obj) {
    objects.push_back(obj);
}

void Scene::addLight(Light const& light) {
    lights.push_back(LightPtr(new Light(light)));
}

void Scene::setEye(Triple const& position) {
    eye = position;
}

unsigned Scene::getNumObject() {
    return objects.size();
}

unsigned Scene::getNumLights() {
    return lights.size();
}

void Scene::setRenderShadows(bool shadows) {
    renderShadows = shadows;
}

void Scene::setRecursionDepth(unsigned depth) {
    recursionDepth = depth;
}

void Scene::setSuperSample(unsigned factor) {
    supersamplingFactor = factor;
}
