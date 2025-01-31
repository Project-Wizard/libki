#include "ki/pclass/ClassType.h"
#include "ki/pclass/TypeSystem.h"
#include "ki/util/exception.h"

namespace ki
{
namespace pclass
{
	IClassType::IClassType(const std::string& name,
		const Type *base_class, const TypeSystem& type_system)
		: Type(name, type_system)
	{
		m_kind = Kind::CLASS;

		// Have we been given a base class?
		if (base_class)
		{
			// Make sure the base class is a class type
			if (base_class->get_kind() != Kind::CLASS)
				throw runtime_error("base_class must be a class type!");

			// Cast the base class up to a IClassType pointer
			m_base_class = dynamic_cast<const IClassType *>(base_class);
			if (!m_base_class)
				throw runtime_error("base_class must inherit IClassType!");
		}
		else
			m_base_class = nullptr;
	}

	bool IClassType::inherits(const Type &type) const
	{
		// Types do not technically inherit from themselves, but it is more useful
		// if they report that they do since these checks are to make sure that objects
		// have a common interface
		if (&type == this)
			return true;

		// If we have a base class, go down the inheritance tree and see if our
		// ancestors inherit from the requested type
		if (m_base_class)
			return m_base_class->inherits(type);

		// We've reached the bottom of the inheritance tree; there is no inheritance.
		return false;
	}
}
}
