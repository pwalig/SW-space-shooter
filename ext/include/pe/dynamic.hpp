#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace pe {
	namespace dynamic {
		template <typename V, typename Scalar = float>
		class value {
		public:
			V val;

			value();

			void update(const Scalar& deltaTime, const V& deltaV);
		};

		template<typename V, typename Scalar>
		inline value<V, Scalar>::value() : val(0)
		{
		}
		template<typename V, typename Scalar>
		inline void value<V, Scalar>::update(const Scalar& deltaTime, const V& deltaV)
		{
			val += deltaV * deltaTime;
		}

		template<>
		inline value<glm::quat, float>::value() : val(glm::quat(glm::vec3(0.0f)))
		{
		}

		template <typename VectorP, typename VectorS, typename VectorF, typename Scalar = float>
		class object {
		public:
			VectorP position;
			VectorS speed;
			Scalar massInverse = Scalar(1);
			Scalar drag = Scalar(0.1);

			object();
			object(const VectorP& initialPosition, const Scalar& mass, const Scalar& drag);

			Scalar getMass();
			void setMass(const Scalar& mass);

			void update(const Scalar& deltaTime, const VectorF& force = VectorF(0));
			void followGoal(const Scalar& deltaTime, const VectorP& goal);

			static object speedDampingSystem(const VectorP& initialPosition, const Scalar& f, const Scalar& z);
		};

		template<typename VectorP, typename VectorS, typename VectorF, typename Scalar>
		inline object<VectorP, VectorS, VectorF, Scalar>::object() :
			position(VectorP(0)), speed(VectorS(0))
		{
		}
		template<typename VectorP, typename VectorS, typename VectorF, typename Scalar>
		inline object<VectorP, VectorS, VectorF, Scalar>::object(
			const VectorP& initialPosition, const Scalar& mass_, const Scalar& drag_
		) : massInverse(Scalar(1) / mass_), drag(drag_), position(initialPosition)
		{
		}
		template<typename VectorP, typename VectorS, typename VectorF, typename Scalar>
		inline Scalar object<VectorP, VectorS, VectorF, Scalar>::getMass()
		{
			return Scalar(1) / massInverse;
		}
		template<typename VectorP, typename VectorS, typename VectorF, typename Scalar>
		inline void object<VectorP, VectorS, VectorF, Scalar>::setMass(const Scalar& mass)
		{
			this->massInverse = Scalar(1) / mass;
		}
		template<typename VectorP, typename VectorS, typename VectorF, typename Scalar>
		inline void object<VectorP, VectorS, VectorF, Scalar>::update(const Scalar& deltaTime, const VectorF& force)
		{
			position += speed * deltaTime;
			speed += (force - (speed * drag)) * deltaTime * massInverse;
		}
		template<typename VectorP, typename VectorS, typename VectorF, typename Scalar>
		inline void object<VectorP, VectorS, VectorF, Scalar>::followGoal(const Scalar& deltaTime, const VectorP& goal)
		{
			update(deltaTime, goal - position);
		}
		template<typename VectorP, typename VectorS, typename VectorF, typename Scalar>
		inline object<VectorP, VectorS, VectorF, Scalar> object<VectorP, VectorS, VectorF, Scalar>::speedDampingSystem(
			const VectorP& initialPosition, const Scalar& f, const Scalar& z)
		{
			return object(
				initialPosition,
				(Scalar)4 * glm::pi<Scalar>() * glm::pi<Scalar>() * f * f, // massInverse
				z / (glm::pi<Scalar>() * f) // drag
			);
		}

		template<>
		inline object<glm::quat, glm::vec3, glm::vec3, float>::object() :
			position(glm::quat(glm::vec3(0.0f))), speed(glm::vec3(0.0f))
		{
		}
		template<>
		inline object<glm::quat, glm::quat, glm::vec3, float>::object() :
			position(glm::quat(glm::vec3(0.0f))), speed(glm::quat(glm::vec3(0.0f)))
		{
		}
		template<>
		inline object<glm::quat, glm::quat, glm::quat, float>::object() :
			position(glm::quat(glm::vec3(0.0f))), speed(glm::quat(glm::vec3(0.0f)))
		{
		}
		template<>
		inline void object<glm::quat, glm::vec3, glm::vec3, float>::update(const float& deltaTime, const glm::vec3& force)
		{
			if (glm::length(speed) != 0.0f) {
				position = glm::normalize(
					glm::rotate(position, glm::length(speed) * deltaTime * massInverse, glm::normalize(speed))
				);
			}
			speed += (force - (speed * drag)) * deltaTime * massInverse;
		}
		template<>
		inline void object<glm::quat, glm::vec3, glm::vec3, float>::followGoal(const float& deltaTime, const glm::quat& goal)
		{
			glm::quat difference = goal * glm::inverse(position);
			glm::vec3 force = glm::axis(difference);
			update(deltaTime, force * glm::angle(difference));
		}

		template<typename Vec, typename Scalar>
		using position = object<Vec, Vec, Vec, Scalar>;

		using position1 = position<glm::vec1, float>;
		using position2 = position<glm::vec2, float>;
		using position3 = position<glm::vec3, float>;
		using position4 = position<glm::vec4, float>;

		template<typename Quat, typename Vec, typename Scalar>
		using rotationA = object<Quat, Vec, Vec, Scalar>;
		template<typename Quat, typename Vec, typename Scalar>
		using rotationB = object<Quat, Quat, Vec, Scalar>;

		using rotation2 = position<float, float>;
		using rotation3A = position<glm::vec3, float>;
		using rotation3B = rotationA<glm::quat, glm::vec3, float>;
		using rotation3C = rotationB<glm::quat, glm::vec3, float>;
		using rotation3D = position<glm::quat, float>;
	}
}