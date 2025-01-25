#pragma once
#include "dynamic.hpp"
#include <stdexcept>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace pe {
	template<typename Scalar = float>
	class rigidbodyI {
	public:
		virtual void update(const Scalar& deltaTime) = 0;
		virtual ~rigidbodyI() = default;
	};

	template <typename PosT, typename RotT, typename TorqT, typename Scalar = float>
	class rigidbody : public rigidbodyI<Scalar> {
	private:
		dynamic::position<PosT, Scalar> position_;
		dynamic::rotationA<RotT, TorqT, Scalar> rotation_;
		PosT netForce = PosT(0);
		TorqT netTorque = TorqT(0);

	public:
		material* mat = &material::default_;
		bool dynamic = true;

		PosT& position();
		PosT& velocity();
		RotT& rotation();
		TorqT& angularVelocity();

		Scalar mass();
		Scalar& massInverse();
		Scalar& drag();
		Scalar momentOfInertia();
		Scalar& momentOfInertiaInverse();
		Scalar& angularDrag();

		void addForce(const PosT& force);
		void addForce(const PosT& force, const PosT& position_);
		void addTorque(const TorqT& torque);
		void update(const Scalar& deltaTime) override;

		PosT getForce(const PosT& force_, const PosT& position_);
		TorqT getTorque(const PosT& force_, const PosT& position_);
		template<typename Matrix>
		Matrix modelMatrix();

		// collision normal must point towards rigidbody b
		static void collide(rigidbody& a, rigidbody& b, const collision::infoD<PosT, Scalar>& ci);
	};

	template<typename PosT, typename RotT, typename TorqT, typename Scalar>
	inline PosT& rigidbody<PosT, RotT, TorqT, Scalar>::position()
	{
		return position_.position;
	}
	template<typename PosT, typename RotT, typename TorqT, typename Scalar>
	inline PosT& rigidbody<PosT, RotT, TorqT, Scalar>::velocity()
	{
		return position_.speed;
	}
	template<typename PosT, typename RotT, typename TorqT, typename Scalar>
	inline RotT& rigidbody<PosT, RotT, TorqT, Scalar>::rotation()
	{
		return rotation_.position;
	}
	template<typename PosT, typename RotT, typename TorqT, typename Scalar>
	inline TorqT& rigidbody<PosT, RotT, TorqT, Scalar>::angularVelocity()
	{
		return rotation_.speed;
	}
	template<typename PosT, typename RotT, typename TorqT, typename Scalar>
	inline Scalar rigidbody<PosT, RotT, TorqT, Scalar>::mass()
	{
		return position_.getMass();
	}
	template<typename PosT, typename RotT, typename TorqT, typename Scalar>
	inline Scalar& rigidbody<PosT, RotT, TorqT, Scalar>::massInverse()
	{
		return position_.massInverse;
	}
	template<typename PosT, typename RotT, typename TorqT, typename Scalar>
	inline Scalar& rigidbody<PosT, RotT, TorqT, Scalar>::drag()
	{
		return position_.drag;
	}
	template<typename PosT, typename RotT, typename TorqT, typename Scalar>
	inline Scalar rigidbody<PosT, RotT, TorqT, Scalar>::momentOfInertia()
	{
		return rotation_.getMass();
	}
	template<typename PosT, typename RotT, typename TorqT, typename Scalar>
	inline Scalar& rigidbody<PosT, RotT, TorqT, Scalar>::momentOfInertiaInverse()
	{
		return rotation_.massInverse;
	}
	template<typename PosT, typename RotT, typename TorqT, typename Scalar>
	inline Scalar& rigidbody<PosT, RotT, TorqT, Scalar>::angularDrag()
	{
		return rotation_.drag;
	}
	template<typename PosT, typename RotT, typename TorqT, typename Scalar>
	inline void rigidbody<PosT, RotT, TorqT, Scalar>::addForce(const PosT& force)
	{
		netForce += force;
	}
	template<typename PosT, typename RotT, typename TorqT, typename Scalar>
	inline void rigidbody<PosT, RotT, TorqT, Scalar>::addForce(const PosT& force, const PosT& position_)
	{
		netForce += getForce(force, position_);
		netTorque += getTorque(force, position_);
	}
	template<typename PosT, typename RotT, typename TorqT, typename Scalar>
	inline void rigidbody<PosT, RotT, TorqT, Scalar>::addTorque(const TorqT& torque)
	{
		netTorque += torque;
	}
	template<typename PosT, typename RotT, typename TorqT, typename Scalar>
	inline void rigidbody<PosT, RotT, TorqT, Scalar>::update(const Scalar& deltaTime)
	{
		position_.update(deltaTime, netForce);
		rotation_.update(deltaTime, netTorque);
		netForce = PosT(0);
		netTorque = TorqT(0);
	}
	template<typename PosT, typename RotT, typename TorqT, typename Scalar>
	inline PosT rigidbody<PosT, RotT, TorqT, Scalar>::getForce(const PosT& force_, const PosT& position_)
	{
		if (position_ == this->position()) return force_;
		PosT direction = glm::normalize(this->position() - position_);
		return direction * glm::dot(force_, direction);
	}
	template<typename PosT, typename RotT, typename TorqT, typename Scalar>
	inline TorqT rigidbody<PosT, RotT, TorqT, Scalar>::getTorque(const PosT& force_, const PosT& position_)
	{
		PosT arm = position_ - this->position();
		return glm::cross(force_, arm);
	}
	template<typename PosT, typename RotT, typename TorqT, typename Scalar>
	template<typename Matrix>
	inline Matrix rigidbody<PosT, RotT, TorqT, Scalar>::modelMatrix()
	{
		throw std::logic_error("specified matrix type not supported");
		return Matrix();
	}

	template<>
	template<>
	inline glm::mat4 rigidbody<glm::vec2, float, float, float>::modelMatrix()
	{
		glm::mat4 M = glm::translate(glm::mat4(1.0f), glm::vec3(position().x, position().y, 0.0f));
		M = glm::rotate(M, rotation(), glm::vec3(0.0f, 0.0f, 1.0f));
		return M;
	}

	template<>
	template<>
	inline glm::mat4 rigidbody<glm::vec3, glm::quat, glm::vec3, float>::modelMatrix()
	{
		glm::mat4 M = glm::translate(glm::mat4(1.0f), position());
		M *= glm::toMat4(rotation());
		return M;
	}

	template<size_t Dim, typename Scalar = float>
	struct rigidbodyTypedef {
		typedef rigidbody<glm::vec<Dim, Scalar>, glm::vec<Dim, Scalar>, glm::vec<Dim, Scalar>, Scalar> type;
	};

	template<typename Scalar>
	struct rigidbodyTypedef<1, Scalar> {
		typedef dynamic::object<Scalar, Scalar, Scalar, Scalar> type;
	};

	template<typename Scalar>
	struct rigidbodyTypedef<2, Scalar> {
		typedef rigidbody<glm::vec<2, Scalar>, Scalar, Scalar, Scalar> type;
	};

	template<typename Scalar>
	struct rigidbodyTypedef<3, Scalar> {
		typedef rigidbody<glm::vec<3, Scalar>, glm::quat, glm::vec<3, Scalar>, Scalar> type;
	};

	template<size_t Dim, typename Scalar = float>
	using rigidbodyD = typename rigidbodyTypedef<Dim, Scalar>::type;

	using rigidbody1D = rigidbodyD<1, float>;
	using rigidbody2D = rigidbodyD<2, float>;
	using rigidbody3D = rigidbodyD<3, float>;
}