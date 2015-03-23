/*
 * Transfo.h
 */

#ifndef TRANSFO_H_
#define TRANSFO_H_

#include "../geometry/transfo.hpp"
#include <map>
#include <vector>
//#include <qnamespace.h>

/**
 * This class contains 3 Matrix (Direct, Indirect, Normal)
 * Provide routine to transform a vector, point with the intial matrix
 */
class Transfo {
public:
	Matrix m_md, m_mi, m_mn;

	Transfo();
	Transfo(StandardMatrix &md);
	void set(StandardMatrix &md);

	Vector vector(double n0, double n1, double n2);
	Vector vector(Point P);

	Vector normal(double n0, double n1, double n2);
	Vector normal(Coord N);

	Vector iNormal(Coord iN);

	Transfo operator*(Transfo &m) const {
		Transfo res;
		res.m_md = m_md * m.m_md;
		res.m_mn = m_mn * m.m_mn;
		return res;
	}

	Transfo operator^(Transfo &m) const {
		Transfo res;
		res.m_mi = m_mi * m.m_mi;
		return res;
	}
};

class TransfoFactory {
public:

	TransfoFactory() : time(0) {};

	///////////////////////////////////////////////////////////////////////////
	/// 				Non mutable Transfo
	///////////////////////////////////////////////////////////////////////////

	/* A non mutable rotation with angle in degre */
	Transfo* getRotationd(char axis, double angle);
	/* A non mutable rotation with angle in radian */
	Transfo* getRotationr(char axis, double rad);
	/* A non mutable translation */
	Transfo* getTranslation(double x, double y, double z);
	/* A non mutable scale with the same coef on x,y,z */
	Transfo* getScale1c(double xyz);
	/* A non mutable scale with 3 coef different */
	Transfo* getScale3c(double x, double y, double z);

	///////////////////////////////////////////////////////////////////////////
	/// 				Control Parameter
	///////////////////////////////////////////////////////////////////////////
	typedef enum { YES, NO, REVERSE } ControlType;

	/**
	 * @param var variable name to register
	 * @param from initial value of the variable
	 * @param to final value of the variable
	 * @param step increment step
	 * @param loop type of control
	 * @param mode '+' => var += step or '*' => var *= step
	 * @return return false if the variable already exist (the existing variable is not affected)
	 */
	bool setControlVariable(char *var, double from , double to , double step, ControlType loop, int offset=0, char mode='+', bool half=false);
	/*
	 * bind a variable to a keybord key
	 * if the variable is enable and the key is pressed -> the variable become disable
	 * -- same thing in opposite way
	 */
	bool bindControlVariable(char *var, char *key );

	/* disable animation on this variable
	 * return false if the variable no exist
	 */
	bool disableVariable(char *var);
	/* enable animation on this variable
	 * return false if the variable no exist
	 */
	bool enableVariable(char *var);
	/*
	 * 	delete a variable
	 */
	bool deleteVariable(char *var);

	/**
	 *  This function make a step on each register parameter
	 *  should be call on a animation loop
	 */
	void step();

	/**
	 * This function manage trigged variables
	 * Should be call for each key event
	 */
	void trigStep(std::string s);

	///////////////////////////////////////////////////////////////////////////
	/// 				Mutable Transfo
	///////////////////////////////////////////////////////////////////////////
	/*
	 * Same as non mutable transfo but transformation are based on register value
	 * if a value is not register NULL is returned
	 */
	Transfo* getRotationd(char axis, char* angle);
	Transfo* getRotationr(char axis, char* rad);
	Transfo* getTranslation(char* x, char* y, char* z);
	Transfo* getScale1c(char* xyz);
	Transfo* getScale3c(char* x, char* y, char* z);

	~TransfoFactory();

private:
	struct ControlParameter {
		double curValue;
		double from;
		double to;
		double step;
		ControlType loop;
		char mode;
		bool way; /* true += step , false -= step */
		bool active;
		int offset;
	};
	enum TransfoType { rotation, translation, scale };
	class ControledTransfo {
	public:
		ControledTransfo() : type(rotation) {};
		ControledTransfo(Transfo tt, TransfoType ttype, char aaxis, double * aangle) :
			t(tt),type(ttype) {rot.axis = aaxis; rot.angle = aangle;};
		ControledTransfo(Transfo tt, TransfoType ttype, double * x, double * y, double * z) :
			t(tt), type(ttype) { trans.x = x; trans.y = y; trans.z = z;};
		Transfo t;
		enum TransfoType type;
		union {
			struct {
				char axis;
				double *angle;
			} rot;
			struct {
				double * x;
				double * y;
				double * z;
			} trans, scale;
		};
	};

	long time;
	std::map<std::string, ControlParameter> vars;
	std::vector<ControledTransfo*> trans;
	std::map<std::string, std::vector<std::string> > binds;
};

#endif /* TRANSFO_H_ */
