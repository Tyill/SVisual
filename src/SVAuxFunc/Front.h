
#pragma once

#include <vector>

namespace SV_Aux {

/// Булевый фронт
	class Front {

	private:
		std::vector<bool> onc;
		int oncSz;

	public:

		Front() : oncSz(0) {}

		/// Польжит фронт
		/// \param en значение
		/// \param id ИД
		/// \return фронт
		bool PosFront(bool en, int id) {
			if (oncSz <= id) {
				onc.resize(id + 1, false);
				oncSz = id + 1;
			}

			if (!onc[id] && en) {
				onc[id] = true;
				return true;
			} else {
				if (!en) onc[id] = false;
				return false;
			}
		}

		/// отрицательный фронт
		/// \param en значение
		/// \param id ИД
		/// \return фронт
		bool NegFront(bool en, int id) {
			if (oncSz <= id) {
				onc.resize(id + 1, true);
				oncSz = id + 1;
			}

			if (!onc[id] && !en) {
				onc[id] = true;
				return true;
			} else {
				if (en) onc[id] = false;
				return false;
			}
		}

	};
}