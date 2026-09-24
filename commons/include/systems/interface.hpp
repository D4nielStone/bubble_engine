#pragma once
#include <glad.h>
#include <memory>
#include "commons_namespace.hpp"
#include "systems/system.hpp"
#include "loaders/image_loader.hpp"
#include "loaders/font.hpp"
#include "util/box.hpp"
#include "elements/button.hpp"
#include "elements/text_box.hpp"
#include "elements/image.hpp"
#include "elements/text.hpp"
#include "util/vector4.hpp"

namespace COMMONS_NS {
	static unsigned int VAO, VBO, EBO;
    struct interface : system {
        void draw(box*);
        static void update_width(box*);
        static void update_height(box*);
        static void updateHDTF(box*, std::function<void(box*)>);
        static bool needs_update(box*);
        void configOpenglState() const;
        void deconfigOpenglState() const;
        void processarDimensaoModular(box*, fvector_type2&, fvector_type2&);
        void organizarLinha(box*, bool, const ivector_type2 , const fvector_type2&, const fvector_type2&, fvector_type2&);
        void process_modular(box*);
        void update_children(box*);
        void chamarFuncoes(box*);
        interface();
        ~interface();

        void gerarBuffers();
        void setup() override;
        void update() override;

        std::unique_ptr<box> m_raiz;
        glm::mat4 projecao_viewport;
    };
}
// \see interface.cpp
