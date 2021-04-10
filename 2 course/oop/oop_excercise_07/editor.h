//
// Created by sakost on 17.12.2020.
//

#ifndef OOP_EXCERCISE_07_EDITOR_H
#define OOP_EXCERCISE_07_EDITOR_H

#include "iomanipulation.h"
#include "figure.h"
#include "concretefactories.h"
#include "concretefigures.h"

#include <vector>
#include <memory>
#include <iomanip>
#include <stack>
#include <map>
#include <cassert>

class Editor {
public:
    using self_type = Editor;
    using self_reference = self_type&;
    using self_pointer = self_type*;
    using self_rvalue_reference = self_type&&;
    using const_self_reference = const self_type&;
    using const_self_pointer = const self_type *;
    using const_self_rvalue_reference = const self_type&&;

    enum class Action{
        ADD,
        DELETE,
    };

    Editor() = default;


    Editor(const Editor &editor){
        m_figures = editor.m_figures;
    }

    Editor(Editor &&editor) noexcept {
        m_figures = std::move(editor.m_figures);
    }

    Editor& operator=(Editor &&editor) noexcept {
        m_figures = std::move(editor.m_figures);
        return *this;
    }

    Editor& operator=(const Editor&editor){
        if(&editor == this) return *this;
        m_figures = editor.m_figures;
        m_actions = editor.m_actions;
        m_delete_cache = editor.m_delete_cache;
        return *this;
    }

    void add_figure(Figure* object){
        auto ptr = std::shared_ptr<Figure>(object);
        add_figure(ptr);
    }

    void add_figure(std::shared_ptr<Figure>& ptr){
        m_actions.push(Action::ADD);
        m_figures.push_back(std::move(ptr));
    }

    void pop_figure(){
        if(m_figures.empty()) return;
        m_actions.push(Action::DELETE);
        m_delete_cache.push(m_figures.back());
        m_figures.pop_back();
    }

    bool undo(){
        if(m_actions.empty()) return false;
        if(m_actions.top() == Action::ADD){
            assert(!m_figures.empty() && "Figures vector may not be empty");
            m_figures.pop_back();
            m_actions.pop();
            return true;
        }
        if(m_actions.top() == Action::DELETE){
            assert(!m_delete_cache.empty() && "Delete cache may not be nullptr");
            m_figures.push_back(m_delete_cache.top());
            m_delete_cache.pop();
            m_actions.pop();
            return true;
        }
        return false;
    }

    [[nodiscard]] const auto& get_figures() const{
        return m_figures;
    }

private:
    std::vector<std::shared_ptr<Figure>> m_figures;

    std::stack<Action> m_actions;
    std::stack<std::shared_ptr<Figure>> m_delete_cache;
};

class EditorSerializer final: public Serializer<Editor> {
public:
    static void serialize(const Editor& editor, std::ostream& out){
        for (const auto &figure : editor.get_figures()) {
            out << std::any_cast<std::string>(figure->getProp("name")) << std::endl;
            out << figure->get_raw_data() << std::endl;
        }
    }
};

class EditorLoader final: public Loader<Editor> {
public:
    static Editor load(std::istream& in){
        std::string line;
        Editor editor;
        SquareFactory s_factory;
        RectangleFactory r_factory;
        TriangleFactory t_factory;
        std::string cache_name;
        while(std::getline(in, line)){
            if(line == "square"){
                cache_name = "square";
            }
            else if(line == "triangle"){
                cache_name = "triangle";
            }
            else if(line == "rectangle"){
                cache_name = "rectangle";
            }
            else{
                if(cache_name == "square"){
                    editor.add_figure(s_factory.create_object(line));
                }
                else if(cache_name == "triangle"){
                    editor.add_figure(t_factory.create_object(line));
                }
                else if(cache_name == "rectangle"){
                    editor.add_figure(r_factory.create_object(line));
                }
            }
        }
        return editor;
    }
};


#endif //OOP_EXCERCISE_07_EDITOR_H
