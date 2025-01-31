#pragma once
#include <cstdint>
#include "ki/pclass/TypeSystem.h"
#include "ki/pclass/Property.h"
#include "ki/pclass/PropertyClass.h"
#include "ki/util/BitStream.h"
#include "ki/util/FlagsEnum.h"

namespace ki
{
namespace serialization
{
	/**
	 * A serializer class used to serialize PropertyClass instances into their binary
	 * representations, and deserialize binary data into PropertyClass instances.
	 */
	class BinarySerializer
	{
	public:
		/**
		 * These flags control how the serializer reads/writes data.
		 */
		enum class flags : uint32_t
		{
			NONE = 0,

			/**
			 * When enabled, the flags the serializer was constructed with are written
			 * into an unsigned 32-bit integer before writing any data.
			 */
			WRITE_SERIALIZER_FLAGS = 0x01,

			/**
			 * When enabled, only properties that were declared with the PUBLIC flag
			 * will be written.
			 */
			WRITE_PUBLIC_ONLY = 0x04,

			/**
			 * When enabled, the serialized data (after the flags, if present) is 
			 * potentially compressed. This is based on an added compression header.
			 */
			COMPRESSED = 0x08
		};

		/**
		 * Construct a new binary serializer.
		 * @param[in] type_system The TypeSystem instance to acquire Type information from.
		 * @param[in] is_file Determines whether or not to write type sizes, and property headers.
		 * @param[in] flags Determines how serialized data is formatted.
		 */
		explicit BinarySerializer(const pclass::TypeSystem &type_system,
			bool is_file, flags flags);
		virtual ~BinarySerializer() = default;

		/**
		 * Serialize an object into a BitStream.
		 * @param[in] object The object to write to the stream.
		 * @param[in] stream The stream to write the object to.
		 */
		void save(const pclass::PropertyClass *object, BitStream &stream);

		/**
		 * Deserialize the contents of a BitStream.
		 * @param[out] dest Where to load the PropertyClass instance into.
		 * @param[in] stream The stream to read the object from.
		 * @param[in] size The size of the stream's available data.
		 */
		void load(std::unique_ptr<pclass::PropertyClass> &dest,
			BitStream &stream, std::size_t size);

	protected:
		/**
		 * @param[in] object The object that is being saved.
		 * @param[in] stream The stream to write the object header to.
		 * @returns Whether or not the object is null.
		 */
		virtual bool presave_object(const pclass::PropertyClass *object, BitStream &stream) const;

		/**
		 * Read an object header, and instantiate the necessary PropertyClass.
		 * @param[out] dest Where to instantiate the PropertyClass.
		 * @param[in] stream The stream to read the object header from.
		 */
		virtual void preload_object(
			std::unique_ptr<pclass::PropertyClass> &dest, BitStream &stream) const;

	private:
		const pclass::PropertyClass *m_root_object;
		const pclass::TypeSystem *m_type_system;
		bool m_is_file;
		flags m_flags;

		void save_object(const pclass::PropertyClass *object, BitStream &stream) const;
		void save_property(const pclass::IProperty &prop, BitStream &stream) const;

		void load_object(
			std::unique_ptr<pclass::PropertyClass> &dest, BitStream &stream) const;
		void load_property(pclass::IProperty &prop, BitStream &stream) const;
	};
}
}

// Make sure the flags enum can be used like a bitflag
MAKE_FLAGS_ENUM(ki::serialization::BinarySerializer::flags);
