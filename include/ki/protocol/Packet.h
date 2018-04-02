#pragma once
#include "../util/Serializable.h"
#include <cstdint>
#include <vector>
#include <sstream>
#include <type_traits>

namespace ki
{
namespace protocol
{
	class Packet : public util::Serializable
	{
	public:
		Packet(bool control = false, uint8_t opcode = 0);
		virtual ~Packet() = default;

		bool is_control() const;
		void set_control(bool control);

		uint8_t get_opcode() const;
		void set_opcode(uint8_t opcode);

		template <typename DataT>
		void set_payload_data(const DataT &data)
		{
			static_assert(std::is_base_of<Serializable, DataT>::value,
				"DataT must derive from Serializable.");

			std::ostringstream oss;
			data.write_to(oss);
			m_payload = std::vector<char>(oss.str().c_str(), data.get_size());
		}

		template <typename DataT>
		DataT *get_payload_data() const
		{
			static_assert(std::is_base_of<Serializable, DataT>::value,
				"DataT must derive from Serializable.");

			std::istringstream iss(m_payload.data());
			DataT *data = new DataT();
			data->read_from(iss);
			return data;
		}

		virtual void write_to(std::ostream &ostream) const final;
		virtual void read_from(std::istream &istream) final;
		virtual size_t get_size() const final;
	private:
		bool m_control;
		uint8_t m_opcode;
		std::vector<char> m_payload;
	};
}
}
