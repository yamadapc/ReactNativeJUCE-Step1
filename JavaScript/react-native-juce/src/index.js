import React, {Component} from 'react';

export function View(props) {
  return props.children;
}

export function Text(props) {
  return props.children;
}

export const ComponentRegistry = {
  components: {},
  register: (name, Klass) => {
    ComponentRegistry.components[name] = Klass;
  },
};
