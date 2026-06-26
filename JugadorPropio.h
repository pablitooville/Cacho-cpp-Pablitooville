#ifndef JUGADORPROPIO_H
#define JUGADORPROPIO_H

#include "Jugador.h"
#include <map>
#include <string>
#include <vector>

class JugadorPropio : public Jugador {
private:   
    std::string nombreEstudiante;
    int PrioridadJuego(const std::string& juego){
        if(juego == "Alalay" || juego == "Grande") return 10;
        if (juego == "Poker") return 9;
        if (juego == "Full") return 8;
        if (juego == "Escalera") return 7;
        if (juego == "Seis") return 6;
        if (juego == "Cinco") return 5;
        if (juego == "Cuatro") return 4;
        if (juego == "Tres") return 3;
        if (juego == "Dos") return 2;
        if (juego == "Uno") return 1;
        return 0;
    }
public:
    JugadorPropio() :Jugador() {
        nombreEstudiante = "Pablo Villegas Becerra";
    }
    JugadorPropio(std::string nom="pablitooville") : Jugador(nom) {
        nombreEstudiante = "Pablo Villegas Becerra";
    }
    int jugar(const std::map<std::string, Marcador>& /*marcadores*/,
        const std::vector<Actuacion> & actuacionesPosibles,
        const std::vector<int>& /*dados*/,
        const Anotacion& /*resultadoPrevio*/) override{
            if(actuacionesPosibles.empty()) return 0; //si no hay chance de hcaer algo pasamos turno insta
            int mejorIndice = 0;
            double mejorEvaluacion = -9999.0; //muy baja para que cualquiera sea mejor
            for(unsigned int i=0; i<actuacionesPosibles.size(); ++i){ //hasta que no hayan más en la lista de actuaciones
                const auto& act = actuacionesPosibles[i]; //pasara cada jugada noma
                double actual = 0.0;
                double bonoSeguridad = act.seRespeta ? 20.0 : -50.0; //si la jugada es buena se asignan pts sino se restan para que no sea buena
                if(!act.indiceDados.empty()) { //si lanzamos de nuevo los dados
                    actual = bonoSeguridad + 10.0 + (4.0 * act.indiceDados.size()); //a mayor dados libres mayor opciones de hacer algo bueno
                }
                else if (!act.anotacion.juego.empty()) { //si la opcion es anotar algo en la hoja
                    int pts = act.anotacion.puntos; //los pts se asignan a lo posible
                    int prioridad = PrioridadJuego(act.anotacion.juego); //calculamos la prioirdad de la jugada

                    if (pts > 0) { //si es que da pts, actual sera una buena jugada sumando pts y multplica la prioridad para que la jugada sea la mas alta
                        actual = (pts * 3.5) + (prioridad * 8.0) + bonoSeguridad;
                    } else { //si no da pts actual pasa a ser mala y se resta la prioridad para tachar los que tengan menor prioridad como"UNO" en vez de "FULL"
                        actual = -200.0 - prioridad; 
                    }
                }
                else{ //si no sale ninguna anterior como "VOLTEAR" la jugada sera valida pero no la mejor seguramente y sino sera negativa y pasara de ella noma
                    actual = 5.0 + bonoSeguridad;
                }
                if(actual > mejorEvaluacion){ //si la jugada de ahora es mejor que la mejor guardada antes se le asigna a la mejor
                    mejorEvaluacion = actual; 
                    mejorIndice = i;
                }
            }
            return mejorIndice;
        }
        std::string getNombreEstudiante() const{
            return nombreEstudiante;
        }
};
#endif
