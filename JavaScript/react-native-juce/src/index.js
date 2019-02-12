import React, {Component} from 'react';

export function View() {
  return this.props.children;
}

export const ComponentRegistry = {
  components: {},
  register: (name, Klass) => {
    ComponentRegistry.components[name] = Klass;
  },
};
