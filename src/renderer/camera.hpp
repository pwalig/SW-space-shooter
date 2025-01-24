#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace ren {
	class camera {
	private:
		glm::mat4 P;
		glm::mat4 V;

		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 look_at = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 up_vector = glm::vec3(0.0f, 1.0f, 0.0f);

		float fov = 70.0f; // vertical field of view
		float aspect_ratio = 16.0f / 9.0f; // width / height calculated aspect ratio
		float near = 0.1f;
		float far = 100.0f;

		void recalculate_P();
		void recalculate_V();

	public:
		camera(const glm::vec3& new_position, const glm::vec3& new_look_at, const glm::vec3& new_up_vector,
			const float& new_fov, const float& new_aspect_ratio, const float& new_near, const float& new_far);
		camera();
		void set_V(const glm::vec3& new_position, const glm::vec3& new_look_at, const glm::vec3& new_up_vector = glm::vec3(0.0f, 1.0f, 0.0f));
		void set_V(const glm::vec3& new_position, const glm::quat& new_rotation);
		void set_position(const glm::vec3& new_position);
		void set_look_at(const glm::vec3& new_look_at, const glm::vec3& new_up_vector = glm::vec3(0.0f, 1.0f, 0.0f));
		void set_direction(const glm::vec3& new_direction, const glm::vec3& new_up_vector = glm::vec3(0.0f, 1.0f, 0.0f));
		void set_rotation(const glm::quat& new_rotation);
		void set_P(const float& new_fov, const float& new_aspect_ratio, const float& new_near, const float& new_far);
		void set_fov(const float& new_fov);
		void set_aspect_ratio(const float& new_aspect_ratio);
		void set_aspect_ratio(const int& width, const int& height);
		void set_clipping(const float& new_near, const float& new_far);

		glm::mat4 get_P() const;
		glm::mat4 get_V() const;
		float get_near() const;
		float get_far() const;
		float get_depth() const;
		float get_vertical_fov() const;
		float get_horizontal_fov() const;
		float get_aspect_ratio() const;

		static camera _default;
	};
}