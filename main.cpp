#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class pessoa {
protected:
    int codigo;
    int cpf[12];
    string nome;
    string email;
    string telefone;
    string whatsapp;

public:
    pessoa() {
        codigo = 0;
        for (int i = 0; i < 12; i++) {
            cpf[i] = 0;
        }
        nome = "";
        email = "";
        telefone = "";
        whatsapp = "";
    }
    void setCodigo(int codin) {
        codigo = codin;
    }
    int getCodigo() const {
        return codigo;
    }
    void setCpf(int cpfin[12]) {
        for (int i = 0; i < 12; i++) {
            cpf[i] = cpfin[i];
        }
    }
    int* getCpf() {
        return cpf;
    }
    void setNome(string nomein) {
        nome = nomein;
    }

    string getNome() const {
        return nome;
    }
    void setEmail(string emailin) {
        email = emailin;
    }
    string getEmail() const {
        return email;
    }
    void setTelefone(string telefonein) {
        telefone = telefonein;
    }
    string getTelefone() const {
        return telefone;
    }
    void setWhatsapp(string whastappin) {
        whatsapp = whastappin;
    }
    string getWhatsapp() const {
        return whatsapp;
    }
};

class secretaria : public pessoa {
private:
    string login;
    string senha;
    short cargo; // 0 - secretária, 1 - secretária-supervisora



public:
    void setLogin(string loginin) {
        login = loginin;
    }

    void setSenha(string senhain) {
        senha = senhain;
    }

};
class medico : public pessoa {
    // Implementação futura
};

class paciente : public pessoa {
    // Implementação futura
};

int main() {
    secretaria sec;
    string nome_temp;

    cout << "Digite o nome da secretária: ";
    cin >> nome_temp;
    sec.setNome(nome_temp);
    cout << "Nome da secretária: " << sec.getNome() << endl;

    return 0;
}
