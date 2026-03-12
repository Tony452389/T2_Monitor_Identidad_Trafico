#pragma once

#include "Evento.h"

void iniciarAnalisis();

void actualizarEstado(const Evento& evento);

void detectarAnomalias(const Evento& evento);

void generarAnomalia(const std::string& descripcion, NivelRiesgo riesgo, const Evento& evento);