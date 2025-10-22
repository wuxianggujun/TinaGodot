/**************************************************************************/
/*  editor_translation.cpp                                                */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "editor_translation.h"

#include "core/io/compression.h"
#include "core/io/file_access_memory.h"
#include "core/io/translation_loader_po.h"
#include "core/string/translation_server.h"
// TinaGodot 2D Lite - Disabled large translation files to save ~100MB
// #include "editor/translations/doc_translations.gen.h"
#include "editor/translations/editor_translations.gen.h"
// #include "editor/translations/extractable_translations.gen.h"
// #include "editor/translations/property_translations.gen.h"

Vector<String> get_editor_locales() {
	Vector<String> locales;

	const EditorTranslationList *etl = _editor_translations;
	while (etl->data) {
		const String &locale = etl->lang;
		locales.push_back(locale);

		etl++;
	}

	return locales;
}

void load_editor_translations(const String &p_locale) {
	const Ref<TranslationDomain> domain = TranslationServer::get_singleton()->get_or_add_domain("godot.editor");

	const EditorTranslationList *etl = _editor_translations;
	while (etl->data) {
		if (etl->lang == p_locale) {
			Vector<uint8_t> data;
			data.resize(etl->uncomp_size);
			const int64_t ret = Compression::decompress(data.ptrw(), etl->uncomp_size, etl->data, etl->comp_size, Compression::MODE_DEFLATE);
			ERR_FAIL_COND_MSG(ret == -1, "Compressed file is corrupt.");

			Ref<FileAccessMemory> fa;
			fa.instantiate();
			fa->open_custom(data.ptr(), data.size());

			Ref<Translation> tr = TranslationLoaderPO::load_translation(fa);

			if (tr.is_valid()) {
				tr->set_locale(etl->lang);
				domain->add_translation(tr);
				break;
			}
		}

		etl++;
	}
}

void load_property_translations(const String &p_locale) {
	// TinaGodot 2D Lite - Disabled property translations to save ~30MB
	// Property translations are not needed for a lightweight 2D editor
}

void load_doc_translations(const String &p_locale) {
	// TinaGodot 2D Lite - Disabled doc translations to save ~50MB
	// Documentation translations are not needed for a lightweight 2D editor
}

void load_extractable_translations(const String &p_locale) {
	// TinaGodot 2D Lite - Disabled extractable translations to save ~20MB
	// Extractable translations are not needed for a lightweight 2D editor
}

Vector<Vector<String>> get_extractable_message_list() {
	// TinaGodot 2D Lite - Disabled extractable translations to save ~20MB
	// Return empty list as extractable translations are disabled
	return Vector<Vector<String>>();
}
