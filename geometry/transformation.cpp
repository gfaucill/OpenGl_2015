/*
 * Transfo.cpp
 */

#include "transformation.hpp"
using namespace std;

Transfo::Transfo() {
	m_mn = m_mi = m_md = Matrix();
}

Transfo::Transfo(StandardMatrix &md) {
	m_md = md;
	m_mi = md.makeInverse();
	m_mn = md.makeNormalTransportMatrix();
}

Vector Transfo::vector(double n0, double n1, double n2) {
	Vector p(n0,n1,n2);
	Vector q = m_md * p;
	return q;
}

Vector Transfo::vector(Point P) {
	return vector(P.coord[0],P.coord[1],P.coord[2]);
}

Vector Transfo::normal(double n0, double n1,double n2) {
	Vector p(n0,n1,n2);
	Vector q = m_mn * p;
	return q;
}

Vector Transfo::normal(Coord N) {
	return normal(N.coord[0],N.coord[1],N.coord[2]);
}

void Transfo::set(StandardMatrix& md) {
	m_md = md;
	m_mi = md.makeInverse();
	m_mn = md.makeNormalTransportMatrix();
}

Vector Transfo::iNormal(Coord iN) {
	Vector p(iN.coord[0],iN.coord[1],iN.coord[2]);
	Vector q = m_mn * p;
	return q;
}

///////////////////////////////////////////////////////////////////////////////
///                   TransfoFactory
///////////////////////////////////////////////////////////////////////////////

int axisToInt(char axis) {
	switch (axis) {
		case 'x' : case 'X' : return 0;
		case 'y' : case 'Y' : return 1;
		case 'z' : case 'Z' : return 2;
		default : return 0;
	}
}

Transfo* TransfoFactory::getRotationd(char axis, double angle) {
	int axe = axisToInt(axis);
	RotationMatrix rot(axe, (angle * M_PI) / 180);
	return new Transfo(rot);
}

Transfo* TransfoFactory::getRotationr(char axis, double rad) {
	int axe = axisToInt(axis);
	RotationMatrix rot(axe, rad);
	return new Transfo(rot);
}

Transfo* TransfoFactory::getTranslation(double x, double y, double z) {
	TranslationMatrix trans(x,y,z);
	return new Transfo(trans);
}

Transfo* TransfoFactory::getScale1c(double xyz) {
	ScaleMatrix scale(xyz, xyz, xyz);
	return new Transfo(scale);
}

Transfo* TransfoFactory::getScale3c(double x, double y, double z) {
	ScaleMatrix scale(x,y,z);
	return new Transfo(scale);
}

bool TransfoFactory::setControlVariable(char* var, double from, double to,
		double step, ControlType loop, int offset, char mode, bool half) {

	if( vars.find(var) != vars.end()) // already in
		return false;

	ControlParameter cp = { (half)?to + from:from , from , to , step, loop,  mode, true/*way*/, true/*active*/, offset};

	vars.insert(std::pair<char*,ControlParameter>(var,cp));
	return true;
}

bool TransfoFactory::bindControlVariable(char *var, char *key)
{
    /* check if the variable exist */
	std::map<std::string, ControlParameter>::iterator vv;
    if( (vv=vars.find(var)) == vars.end()) // not in
        return false;

    vv->second.active = false;
    /* insert in the map */
    std::map<string, std::vector<string> >::iterator v;
    if((v = binds.find(key)) == binds.end()) { // Not in
        binds.insert(std::pair<string, std::vector<string> >(key, std::vector<string>()));
    }
    binds.find(key)->second.push_back(var); // value can be duplicate

    for (std::map<string, vector<string> >::iterator it=binds.begin(); it!=binds.end(); ++it) {
        //std::cout << it->first << ' ' ;
        vector<string> ss = it->second;
        for (vector<string>::iterator itt = ss.begin(); itt != ss.end(); ++itt) {
            //std::cout << " => " << *itt << endl;
        }
    }
   // cout << endl << endl;
    return true;
}

bool TransfoFactory::disableVariable(char* var) {
	std::map<string, ControlParameter>::iterator v;
	if( (v = vars.find(string(var))) == vars.end() ) // not in
			return false;

	v->second.active = false;
	return true;
}

bool TransfoFactory::enableVariable(char* var) {
	std::map<string, ControlParameter>::iterator v;
	if( (v = vars.find(string(var))) == vars.end() ) // not in
			return false;

	v->second.active = true;
	return true;
}

/* TODO peut poser des problemes si des transfo sont encore utilisé*/
bool TransfoFactory::deleteVariable(char* var) {
	std::map<string, ControlParameter>::iterator v;
	if((v = vars.find(string(var))) == vars.end()) // not in
		return false;
	vars.erase(var);
	return true;
}

void TransfoFactory::step() {
	for (std::map<string, ControlParameter>::iterator it=vars.begin(); it!=vars.end(); ++it) {
		ControlParameter * cp = &it->second;

		if (cp->active) {
			if (cp->offset <= time) {
				switch (cp->loop) {
					case YES :
						if (cp->mode == '+') {
							if(cp->curValue + cp->step > cp->to)
								cp->curValue = cp->from;
							cp->curValue += cp->step;
						} else if (cp->mode == '*' ) {
							if(cp->curValue * cp->step > cp->to)
								cp->curValue = cp->from;
							cp->curValue *= cp->step;
						}
						break;
					case NO :
						if (cp->mode == '+') {
							if ( cp->step > 0 ) {
								if (cp->curValue + cp->step < cp->to )
									cp->curValue += cp->step;
							}
							else {
								if (cp->curValue + cp->step > cp->to)
									cp->curValue += cp->step;
							}
						}
						else if (cp->mode == '*') {
							if( cp->curValue * cp->step < cp->to)
								cp->curValue *= cp->step;
						}
						break;
					case REVERSE :
						if (cp->curValue > cp->to ) cp->way = false;
						else if (cp->curValue < cp->from) cp->way = true;
						if(cp->mode == '+') {
							if (cp->way) cp->curValue += cp->step;
							else cp->curValue -= cp->step;
						}
						else if (cp->mode == '*') {
							if (cp->way) cp->curValue *= cp->step;
							else cp->curValue /= cp->step;
						}
						break;
				}
			}
		}
	}

	for (std::vector<ControledTransfo*>::iterator it = trans.begin() ; it != trans.end(); ++it) {
		ControledTransfo * ct = *it;
		if (ct->type  == rotation) {
			RotationMatrix r(axisToInt(ct->rot.axis), *(ct->rot.angle));
			ct->t.set(r);
		}
		else if (ct->type == translation) {
			TranslationMatrix t(*ct->trans.x,*ct->trans.y,*ct->trans.z);
			ct->t.set(t);
		}
		else if (ct->type == scale) {
			ScaleMatrix s(*ct->scale.x,*ct->scale.y,*ct->scale.z);
			ct->t.set(s);
		}
	}
	// increment time
	time++;
}

void TransfoFactory::trigStep(string s)
{
	//std::cout << s << std::endl;
	std::map<string, std::vector<string> >::iterator v;
    if ((v = binds.find(s)) != binds.end()) {
    	for (vector<string>::iterator it = v->second.begin(); it != v->second.end(); ++it) {
            std::map<string, ControlParameter>::iterator vv;
            if ((vv = vars.find(*it)) != vars.end()) {
                vv->second.active = (vv->second.active)?false:true;
                if(vv->second.loop == TransfoFactory::REVERSE) {
                	if(vv->second.active)
                		vv->second.way = (vv->second.way)?false:true;
                }
                else {
                	vv->second.step = -vv->second.step;
                	double from = vv->second.from;
                	vv->second.from = vv->second.to;
                	vv->second.to = from;
                }
            }
        }
    }
}

Transfo* TransfoFactory::getRotationd(char axis, char* angle) {
	std::map<string, ControlParameter>::iterator v;
	if((v = vars.find(string(angle))) == vars.end()) // not in
		return NULL;

	double ang = v->second.curValue;
	int axe = axisToInt(axis);
	RotationMatrix rot(axe, (ang * M_PI) / 180);
	ControledTransfo * ct = new ControledTransfo(Transfo(rot), rotation, axis, &v->second.curValue);
	trans.push_back(ct);
	return &ct->t;
}

Transfo* TransfoFactory::getRotationr(char axis, char* rad) {
	std::map<string, ControlParameter>::iterator v;
	if((v = vars.find(rad)) == vars.end()) // not in
		return NULL;

	double ang = v->second.curValue;
	int axe = axisToInt(axis);
	RotationMatrix rot(axe, ang);
	ControledTransfo * ct = new ControledTransfo(Transfo(rot), rotation, axis, &v->second.curValue);
	trans.push_back(ct);

	return &ct->t;
}

Transfo* TransfoFactory::getTranslation(char* x, char* y, char* z) {
	std::map<string, ControlParameter>::iterator mapX;
	if((mapX = vars.find(x)) == vars.end()) // not in
		return NULL;
	std::map<string, ControlParameter>::iterator mapY;
	if((mapY = vars.find(y)) == vars.end()) // not in
		return NULL;
	std::map<string, ControlParameter>::iterator mapZ;
	if((mapZ = vars.find(z)) == vars.end()) // not in
		return NULL;

	TranslationMatrix tran(mapX->second.curValue, mapY->second.curValue, mapZ->second.curValue);
	ControledTransfo * ct = new ControledTransfo(tran, translation, &mapX->second.curValue,
			&mapY->second.curValue, &mapZ->second.curValue);
	trans.push_back(ct);

	return &ct->t;
}

Transfo* TransfoFactory::getScale1c(char* xyz) {
	std::map<string, ControlParameter>::iterator mapXYZ;
	if((mapXYZ = vars.find(xyz)) == vars.end()) // not in
		return NULL;

	ScaleMatrix scaleM(mapXYZ->second.curValue, mapXYZ->second.curValue, mapXYZ->second.curValue);
	ControledTransfo * ct = new ControledTransfo(scaleM, scale, &mapXYZ->second.curValue,
			&mapXYZ->second.curValue,&mapXYZ->second.curValue);
	trans.push_back(ct);

	return &ct->t;
}

Transfo* TransfoFactory::getScale3c(char* x, char* y, char* z) {
	std::map<string, ControlParameter>::iterator mapX;
	if((mapX = vars.find(x)) == vars.end()) // not in
		return NULL;
	std::map<string, ControlParameter>::iterator mapY;
	if((mapY = vars.find(y)) == vars.end()) // not in
		return NULL;
	std::map<string, ControlParameter>::iterator mapZ;
	if((mapZ = vars.find(z)) == vars.end()) // not in
		return NULL;

	ScaleMatrix scaleM(mapX->second.curValue, mapY->second.curValue, mapZ->second.curValue);
	ControledTransfo * ct = new ControledTransfo(scaleM, scale, &mapX->second.curValue,
			&mapY->second.curValue, &mapZ->second.curValue);

	trans.push_back(ct);

	return &ct->t;
}

TransfoFactory::~TransfoFactory() {
	for (std::vector<ControledTransfo*>::iterator it = trans.begin() ; it != trans.end(); ++it) {
		delete *it;
	}
}
