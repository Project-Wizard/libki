#pragma once
#include <memory>
#include <json.hpp>
#include "ki/pclass/Property.h"
#include "ki/pclass/TypeSystem.h"

namespace ki
{
namespace serialization
{
	/**
	 * TODO: Documentation
	 */
	class JsonSerializer
	{
		static const int FILE_INDENT_VALUE = 2;

	public:
		explicit JsonSerializer(const pclass::TypeSystem &type_system, bool is_file);
		virtual ~JsonSerializer() = default;

		std::string save(const pclass::PropertyClass *object) const;
		void load(std::unique_ptr<pclass::PropertyClass> &dest, const std::string &json_string) const;

	protected:
		virtual bool presave_object(nlohmann::json &j, const pclass::PropertyClass *object) const;
		virtual bool preload_object(std::unique_ptr<pclass::PropertyClass> &dest, nlohmann::json &j) const;

	private:
		const pclass::TypeSystem *m_type_system;
		bool m_is_file;

		nlohmann::json save_object(const pclass::PropertyClass *object) const;
		void save_property(nlohmann::json &j, const pclass::IProperty &prop) const;

		void load_object(std::unique_ptr<pclass::PropertyClass> &dest, nlohmann::json &j) const;
		void load_property(pclass::IProperty &prop, nlohmann::json &j) const;
	};
}
}
