#pragma once

template<typename T, typename... Args>
inline T call(const char* func, Args... args) {
	return reinterpret_cast<T(__fastcall*)(Args...)>(GetProcAddress(GetModuleHandleA(xorstr_("GameAssembly.dll")), func))(args...);
}

namespace System {
	class Object_ {
	public:

	};
	template<typename T = void*>
	class Array {
	public:
		uint32_t size( ) {
			if (!this) return 0;
			return *reinterpret_cast<uint32_t*>(this + 0x18);
		}
		T get(int idx) {
			if (!this) return T{};
			return *reinterpret_cast<T*>(this + (0x20 + (idx * 0x8)));
		}
		void add(int idx, T value) {
			if (!this) return;
			*reinterpret_cast<T*>(this + (0x20 + (idx * 0x8))) = value;
		}
	};
	class String : public Object_ {
	public:
		char pad_0000[0x10];
		int len;
		wchar_t buffer[0];

		static String* New(const char* str) {
			return call<String*, const char*>(xorstr_("il2cpp_string_new"), str);
		}
	};
	template<typename T = void*>
	struct List {
	public:
		char pad_0000[0x10];
		void* buffer;
		uint32_t size;

		T get(uint32_t idx) {
			if (!this) return nullptr;

			if (idx > this->size) return nullptr;

			void* items = this->buffer;

			if (!items) return nullptr;

			return *reinterpret_cast<T*>((uint64_t)items + (0x20 + (idx * 0x8)));
		}
	};
}

class Il2CppType {
public:
	char* name( ) {
		return call<char*, Il2CppType*>(xorstr_("il2cpp_type_get_name"), this);
	}
};

class Il2CppMethod {
public:
	uint32_t paramCount( ) {
		return call<uint32_t, Il2CppMethod*>(xorstr_("il2cpp_method_get_param_count"), this);
	}

	Il2CppType* retType( ) {
		return call<Il2CppType*, Il2CppMethod*>(xorstr_("il2cpp_method_get_return_type"), this);
	}

	Il2CppType* getParam(uint32_t idx) {
		return call<Il2CppType*, Il2CppMethod*, uint32_t>(xorstr_("il2cpp_method_get_param"), this, idx);
	}

	char* name( ) {
		return call<char*, Il2CppMethod*>(xorstr_("il2cpp_method_get_name"), this);
	}
};

uint64_t il2cpp_resolve_icall(const char* str) {
	return call<uint64_t, const char*>(xorstr_("il2cpp_resolve_icall"), str);
}
uint64_t il2cpp_object_new(uint64_t klass) {
	return call<uint64_t, uint64_t>(xorstr_("il2cpp_object_new"), klass);
}
class Il2CppField {
public:
	char* name( ) {
		return call<char*, Il2CppField*>(xorstr_("il2cpp_field_get_name"), this);
	}

	uint32_t offset( ) {
		return call<uint32_t, Il2CppField*>(xorstr_("il2cpp_field_get_offset"), this);
	}
};

class Il2CppClass {
public:
	class Il2CppImage* image; //0x0000
	char pad_0008[8]; //0x0008
	char* name; //0x0010
	char* namespaze; //0x0018
	char pad_0020[152]; //0x0020
	void* static_fields; //0x00B8

	Il2CppMethod* methods(void* iter) {
		return call<Il2CppMethod*, Il2CppClass*, void*>(xorstr_("il2cpp_class_get_methods"), this, iter);
	}

	Il2CppField* fields(void* iter) {
		return call<Il2CppField*, Il2CppClass*, void*>(xorstr_("il2cpp_class_get_fields"), this, iter);
	}
}; //Size: 0x00C0

class Il2CppImage {
public:
	char* assemblyName; //0x0000
	char* name; //0x0008

	uint64_t classcount( ) {
		return call<uint64_t, Il2CppImage*>(xorstr_("il2cpp_image_get_class_count"), this);
	}

	Il2CppClass* get_class(uint64_t idx) {
		return call<Il2CppClass*, Il2CppImage*, uint64_t>(xorstr_("il2cpp_image_get_class"), this, idx);
	}
}; //Size: 0x0010
template<typename T = System::Object_>
System::Array<T*>* il2cpp_array_new(Il2CppClass* klazz, uint64_t length) {
	auto ret = call<System::Array<T*>*, Il2CppClass*, uint64_t>(xorstr_("il2cpp_array_new"), klazz, length);
	return ret;
}

class Il2CppAssembly {
public:
	uint64_t buffer;
};

class Il2CppDomain {
public:
	size_t assemblyCount( ) {
		size_t size = 0;
		auto assemblies = call<Il2CppAssembly**, Il2CppDomain*, void*>(xorstr_("il2cpp_domain_get_assemblies"), this, &size);

		return size;

	}

	Il2CppAssembly** assemblies( ) {
		size_t size = 0;

		return call<Il2CppAssembly**, Il2CppDomain*, void*>(xorstr_("il2cpp_domain_get_assemblies"), this, &size);
	}
};

Il2CppDomain* il2cpp_domain_get( ) {
	return call<Il2CppDomain*>(xorstr_("il2cpp_domain_get"));
}

class default_t {
public:
	template<typename T>
	operator T( ) const { return T( ); }
};
default_t const defaultt = default_t( );
#define NP(type) type nonptr = defaultt; if(!this) return nonptr;

static std::map<uint32_t, uint64_t> offsets = std::map<uint32_t, uint64_t>( );

Il2CppClass* klass(uint32_t path) {
	if (map_contains_key(offsets, path))
		return reinterpret_cast<Il2CppClass*>(offsets[path]);

	Il2CppDomain* domain = il2cpp_domain_get( );
	Il2CppAssembly** assemblies = domain->assemblies( );

	for (int i = 0; i < domain->assemblyCount( ); i++) {
		Il2CppImage* image = *reinterpret_cast<Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
		for (int c = 0; c < image->classcount( ); c++) {
			std::string temp(image->assemblyName);
			temp.erase(temp.find(xorstr_(".dll")), 4);

			Il2CppClass* klass = image->get_class(c);
			char* name = klass->name;
			char* ns = klass->namespaze;
			if (std::string(ns).empty( ))
				temp = temp + xorstr_("::") + name;
			else
				temp = temp + xorstr_("::") + ns + xorstr_("::") + name;

			if (path == RUNTIME_CRC32(temp.c_str( ))) {
				uint64_t ptr = std::uint64_t(klass);

				offsets.insert(std::make_pair(path, ptr));
				return klass;
			}
		}
	}

	return nullptr;
}

uint64_t static_field(uint32_t path) {
	if (map_contains_key(offsets, path))
		return std::uint32_t(offsets[path]);

	Il2CppDomain* domain = il2cpp_domain_get( );
	Il2CppAssembly** assemblies = domain->assemblies( );

	for (int i = 0; i < domain->assemblyCount( ); i++) {
		Il2CppImage* image = *reinterpret_cast<Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
		for (int c = 0; c < image->classcount( ); c++) {
			std::string temp(image->assemblyName);
			temp.erase(temp.find(xorstr_(".dll")), 4);

			Il2CppClass* klass = image->get_class(c);
			char* name = klass->name;
			char* ns = klass->namespaze;
			if (std::string(ns).empty( ))
				temp = temp + xorstr_("::") + name;
			else
				temp = temp + xorstr_("::") + ns + xorstr_("::") + name;

			Il2CppField* field;
			void* iter = NULL;
			while (field = klass->fields(&iter)) {
				if (!field) continue;

				std::string t(temp + xorstr_("::") + field->name( ));
				if (RUNTIME_CRC32(t.c_str( )) == path) {
					uint32_t off = field->offset( );

					uint64_t ptr = *reinterpret_cast<uint64_t*>(std::uint64_t(klass->static_fields) + off);
					offsets.insert(std::make_pair(path, ptr));

					return off;
				}
			}
		}
	}

	return 0;
}


uint64_t field(uint32_t path) {
	if (map_contains_key(offsets, path))
		return std::uint32_t(offsets[path]);

	Il2CppDomain* domain = il2cpp_domain_get( );
	Il2CppAssembly** assemblies = domain->assemblies( );

	for (int i = 0; i < domain->assemblyCount( ); i++) {
		Il2CppImage* image = *reinterpret_cast<Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
		for (int c = 0; c < image->classcount( ); c++) {
			std::string temp(image->assemblyName);
			temp.erase(temp.find(xorstr_(".dll")), 4);

			Il2CppClass* klass = image->get_class(c);
			char* name = klass->name;
			char* ns = klass->namespaze;
			if (std::string(ns).empty( ))
				temp = temp + xorstr_("::") + name;
			else
				temp = temp + xorstr_("::") + ns + xorstr_("::") + name;

			Il2CppField* field;
			void* iter = NULL;
			while (field = klass->fields(&iter)) {
				if (!field) continue;

				std::string t(temp + xorstr_("::") + field->name( ));
				if (RUNTIME_CRC32(t.c_str( )) == path) {
					uint32_t off = field->offset( );
					offsets.insert(std::make_pair(path, off));

					return off;
				}
			}
		}
	}

	return 0;
}

uint64_t method(uint32_t path) {
	if (map_contains_key(offsets, path))
		return offsets[path];

	Il2CppDomain* domain = il2cpp_domain_get( );
	Il2CppAssembly** assemblies = domain->assemblies( );

	for (int i = 0; i < domain->assemblyCount( ); i++) {
		Il2CppImage* image = *reinterpret_cast<Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
		for (int c = 0; c < image->classcount( ); c++) {
			std::string temp(image->assemblyName);
			temp.erase(temp.find(xorstr_(".dll")), 4);

			Il2CppClass* klass = image->get_class(c);
			if (!klass) continue;

			char* name = klass->name;
			char* ns = klass->namespaze;
			if (std::string(ns).empty( ))
				temp = temp + xorstr_("::") + name;
			else
				temp = temp + xorstr_("::") + ns + xorstr_("::") + name;

			Il2CppMethod* mthd;
			void* iter = NULL;
			while (mthd = klass->methods(&iter)) {
				if (!mthd) continue;

				std::string temp2(temp + xorstr_("::") + mthd->name( ));

				if (mthd->paramCount( ) > 0) {
					temp2 = temp2 + xorstr_("(");
					for (int p = 0; p < mthd->paramCount( ); p++) {
						std::string t(mthd->getParam(p)->name( ));
						t = t.substr(t.find(xorstr_(".")) + 1);
						temp2 = temp2 + t + xorstr_(",");
					}
					std::string t(mthd->retType( )->name( ));
					temp2 = temp2.substr(0, temp2.size( ) - 1);
					temp2 = temp2 + xorstr_("): ") + t.substr(t.find(".") + 1);
				}
				else {
					std::string t(mthd->retType( )->name( ));
					temp2 = temp2 + xorstr_("(): ") + t.substr(t.find(".") + 1);
				}

				if (RUNTIME_CRC32(temp2.c_str( )) == path) {
					offsets.insert(std::make_pair(path, std::uint64_t(mthd)));
					return std::uint64_t(mthd);
				}
			}
		}
	}

	return 0;
}
#if defined _M_IX86
using ptr_t = uint32_t;
#elif defined _M_X64
using ptr_t = uint64_t;
#endif

enum UnmanagedCallingConvention {
	UnmanagedCdecl,
	UnmanagedStdcall,
	UnmanagedFastcall,
};

template<typename t_Function>
class UnmanagedPointer {
public:

	template<typename... t_FunctionParameters>
	auto operator()(t_FunctionParameters... params) {
		using result_type = decltype(std::declval<t_Function>( )(std::declval<t_FunctionParameters>( )...));
		using function_cdecl_ptr_t = result_type(__cdecl*)(t_FunctionParameters...);
		using function_stdcall_ptr_t = result_type(__stdcall*)(t_FunctionParameters...);
		using function_fastcall_ptr_t = result_type(_fastcall*)(t_FunctionParameters...);

		switch (this->m_CallingConvention) {
		case UnmanagedCdecl:
			return reinterpret_cast<function_cdecl_ptr_t>(this->m_Address)(params...);
		case UnmanagedStdcall:
			return reinterpret_cast<function_stdcall_ptr_t>(this->m_Address)(params...);
		case UnmanagedFastcall:
			return reinterpret_cast<function_fastcall_ptr_t>(this->m_Address)(params...);
		}

		return reinterpret_cast<function_stdcall_ptr_t>(this->m_Address)(params...);
	}

	UnmanagedPointer(ptr_t dwAddress, UnmanagedCallingConvention unmCallingConvention) {
		this->m_Address = dwAddress;
		this->m_CallingConvention = unmCallingConvention;
	}
private:
	ptr_t m_Address;
	UnmanagedCallingConvention m_CallingConvention;
};

#define STATIC_FUNCTION(method_path,name,ta) static inline UnmanagedPointer<ta> name = { METHOD(method_path), UnmanagedStdcall }

#define OFFSET(path) field(STATIC_CRC32(path))

#define METHOD(path) *reinterpret_cast<uint64_t*>(method(STATIC_CRC32(path)))

#define METHOD_INF(path) reinterpret_cast<void*>(method(STATIC_CRC32(path)))

#define CLASS(path) klass(STATIC_CRC32(path))

#define STATIC_FIELD(path) static_field(STATIC_CRC32(path))

#define FIELD(field_path,name,type) type& name() { \
		NP(type) \
		static auto off = OFFSET(field_path); \
		return *reinterpret_cast<type*>(this + off); }

#define OFFSET_FIELD(offset,name,type) type& name() { \
		NP(type) \
		static auto off = offset; \
		return *reinterpret_cast<type*>(this + off); }

#define ASSIGN_HOOK(method_path,hook) hook = reinterpret_cast<decltype(hook)>(METHOD(method_path))

void init_classes( ) {
	Il2CppDomain* domain = il2cpp_domain_get( );
	Il2CppAssembly** assemblies = domain->assemblies( );

	for (int i = 0; i < domain->assemblyCount( ); i++) {
		Il2CppImage* image = *reinterpret_cast<Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
		for (int c = 0; c < image->classcount( ); c++) {
			std::string temp(image->assemblyName);
			temp.erase(temp.find(xorstr_(".dll")), 4);

			Il2CppClass* klass = image->get_class(c);
			char* name = klass->name;
			char* ns = klass->namespaze;
			if (std::string(ns).empty( ))
				temp = temp + xorstr_("::") + name;
			else
				temp = temp + xorstr_("::") + ns + xorstr_("::") + name;

			uint64_t ptr = std::uint64_t(klass);

			offsets.insert(std::make_pair(RUNTIME_CRC32(temp.c_str( )), ptr));
		}
	}
}
void init_methods( ) {
	Il2CppDomain* domain = il2cpp_domain_get( );
	Il2CppAssembly** assemblies = domain->assemblies( );

	for (int i = 0; i < domain->assemblyCount( ); i++) {
		Il2CppImage* image = *reinterpret_cast<Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
		for (int c = 0; c < image->classcount( ); c++) {
			std::string temp(image->assemblyName);
			temp.erase(temp.find(xorstr_(".dll")), 4);

			Il2CppClass* klass = image->get_class(c);
			if (!klass) continue;

			char* name = klass->name;
			char* ns = klass->namespaze;
			if (std::string(ns).empty( ))
				temp = temp + xorstr_("::") + name;
			else
				temp = temp + xorstr_("::") + ns + xorstr_("::") + name;

			Il2CppMethod* mthd;
			void* iter = NULL;
			while (mthd = klass->methods(&iter)) {
				if (!mthd) continue;

				std::string temp2(temp + xorstr_("::") + mthd->name( ));

				if (mthd->paramCount( ) > 0) {
					temp2 = temp2 + xorstr_("(");
					for (int p = 0; p < mthd->paramCount( ); p++) {
						std::string t(mthd->getParam(p)->name( ));
						t = t.substr(t.find(xorstr_(".")) + 1);
						temp2 = temp2 + t + xorstr_(",");
					}
					std::string t(mthd->retType( )->name( ));
					temp2 = temp2.substr(0, temp2.size( ) - 1);
					temp2 = temp2 + xorstr_("): ") + t.substr(t.find(".") + 1);
				}
				else {
					std::string t(mthd->retType( )->name( ));
					temp2 = temp2 + xorstr_("(): ") + t.substr(t.find(".") + 1);
				}

				offsets.insert(std::make_pair(RUNTIME_CRC32(temp2.c_str( )), std::uint64_t(mthd)));
			}
		}
	}
}
void init_fields( ) {
	Il2CppDomain* domain = il2cpp_domain_get( );
	Il2CppAssembly** assemblies = domain->assemblies( );

	for (int i = 0; i < domain->assemblyCount( ); i++) {
		Il2CppImage* image = *reinterpret_cast<Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));
		for (int c = 0; c < image->classcount( ); c++) {
			std::string temp(image->assemblyName);
			temp.erase(temp.find(xorstr_(".dll")), 4);

			Il2CppClass* klass = image->get_class(c);
			char* name = klass->name;
			char* ns = klass->namespaze;
			if (std::string(ns).empty( ))
				temp = temp + xorstr_("::") + name;
			else
				temp = temp + xorstr_("::") + ns + xorstr_("::") + name;

			Il2CppField* field;
			void* iter = NULL;
			while (field = klass->fields(&iter)) {
				if (!field) continue;

				std::string t(temp + xorstr_("::") + field->name( ));
				uint32_t off = field->offset( );
				offsets.insert(std::make_pair(RUNTIME_CRC32(t.c_str( )), off));
			}
		}
	}
}