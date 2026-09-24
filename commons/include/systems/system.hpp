#pragma once
#include "commons_namespace.hpp"
#include <memory>
#include "core/ecs.hpp"

namespace COMMONS_NS {
    /**
     * @struct system
     * Classe base para system do loop principal
     */
    struct system {
        /**
         * Constructor virtual da classe Sishasa
         * @param nome Nome do system
         */
        system() = default;

        /**
         * @brief Destrutor virtual para garantir a destruição adequada das subclasses
         */
        virtual ~system() = default;

        /**
         * @brief Inicializa o system
         */
        virtual void setup(const std::shared_ptr<ecs>&) = 0;

        /**
         * @brief Atualiza o system dentro do loop principal
         */
        virtual void update(const std::shared_ptr<ecs>&) = 0;
    };
}
